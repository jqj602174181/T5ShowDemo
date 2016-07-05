#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include "PrjDef.h"
#include "GlobalDef.h"
#include "LinuxAux.h"
#include "LinuxAuxCmd.h"
#include "Debug.h"

struct termio g_stOldTermAttr;//定义全局的终端属性



typedef struct AuxOp_t
{
     S32   nSimType; //仿真类型 
     S8  auxLead[ MAX_LEN ];//开始写数据标识 
     S32   auxLeadLen;
     S8  auxEnd[ MAX_LEN];   //结束写数据标识
     S32   auxEndLen;
    
}AuxOp;


static int bp_pra[] = { 9600, 4800, 2400, 1200, 600, 300, 19200, 38400, 115200 }; /*波特率参数*/ 
static int bp_ser[][ 9 ] = {{0, 1, 2, 3, 5, 6, 10, 11, 12}, {8, 7, 6, 5, 4, 3, 9, 8, 8}};             /*波特率参数序号*/ 
static int aux_ser[] = {AUX1, AUX2, AUX3, AUX4};                                                /*辅口标识序号*/ 
static int wyse_ser[] = {45, 54, 60, 45};                                            /*WYSE的辅口标识序号*/ 
static char star_ser[] = {'Y', 'Z', 'X', 'V'};                                                                 /*实达辅口标识序号*/

/*对于辅口操作涉及的一些全局变量有这个变量统一管理*/
AuxOp g_AuxOp;


static S32 g_AuxFd = INVALID_DESCRIPTOR;//定义全局的文件标识符

void SetAuxFd(S32 Fd)
{
    g_AuxFd = Fd;
}

S32 WriteBlock(S32 fd, U8 *pData, S32 nDataLen)
{
    return write( fd, pData, nDataLen );
}

S32 WriteUnBlock(S32 fd, U8 *pData, S32 nDataLen)
{
    int n, Len = 0;

    struct timeval tv_timeout;

    fd_set fs_write;

    FD_ZERO (&fs_write);

    FD_SET (fd, &fs_write);

    tv_timeout.tv_sec =1;/*超时时间3秒*/

    tv_timeout.tv_usec = 0;


    DbgPrintf("WriteUnblock\n");

    n = select (fd + 1, NULL, &fs_write, NULL, &tv_timeout);

    if(n) 

    {

        Len= write (fd, pData, nDataLen);

        if (Len> 0) 
        {
            return Len;
        }

    }

    else 

    {

        tcflush (fd, TCOFLUSH);     /* flush all output data */

    }

    return Len;
}

S32 CT_SendDataToTransPort( U8* szData, S32 nDataLen )
{
    S32 nWriteLen = 0;
    S32 nTempLen = 0;

    /*验证参数*/
    if( INVALID_DESCRIPTOR ==  g_AuxFd||  NULL ==  szData || nDataLen < 1 )
    {

        return RET_INVALID_PARAM;
    }

    while( nWriteLen < nDataLen )
    {
#ifdef OPER_AUX_UN_BLOCK
        nTempLen = WriteUnBlock(g_AuxFd, szData + nWriteLen, nDataLen - nWriteLen);        
#else
        nTempLen = WriteBlock( g_AuxFd, szData + nWriteLen, nDataLen - nWriteLen );
#endif
        if( nTempLen < 0 )
        {
            break;
        }

        nWriteLen += nTempLen;
    }

    return nWriteLen;
}

void ClearReceiveBuffer( void )
{
    if( INVALID_DESCRIPTOR != g_AuxFd )
    {
        tcflush( g_AuxFd, TCIFLUSH );
    }    
}

static char *ClearInvisibleCharInStr(char *pStr )
{
      int    iLen;
      int   i, k;
      
      /*  输入为空则直接返回*/
      if( !pStr ) 
      {
           return NULL;
      }
       
      iLen = (int)strlen(pStr);
      for(i = 0, k = 0; i < iLen; i++)
      {
            if( ' ' != pStr[i] && 9 != pStr[i] && 10 !=pStr[i] && 13 != pStr[i] )
            {
                  pStr[k++] = pStr[i];
             }
      }
      if(k != iLen)
      {
            pStr[k] = 0;
      }
      return pStr;
}

static S32 ResetAux( void )
{
    /*验证参数*/
    if( INVALID_DESCRIPTOR  == g_AuxFd )
    {
        return RET_INVALID_PARAM;
    }

    /*恢复终端原始配置*/
    if( ioctl( g_AuxFd ,TCSETAF, &g_stOldTermAttr ) < 0 )
    {
        return RET_ERROR;
    }

    return RET_SUCCESS;
}


static S32 SetTerm( S32 nFd, S32 nBaud )
{
    S32  nRetVal = RET_ERROR;
    struct termio stTermAttr; 
   
    /*验证参数*/
    if( INVALID_DESCRIPTOR == nFd )
    {
        return RET_INVALID_PARAM;
    }
    
    /*获得原始的终端参数*/
    nRetVal = ioctl( nFd, TCGETA, &g_stOldTermAttr );
     if( nRetVal < 0 )
     {
          return RET_ERROR;
     }

     switch( nBaud )
     {
        case 300:
            nBaud = B300;
            break;
        case 1200:
            nBaud = B1200;
            break;
        case 2400:
            nBaud = B2400;
            break;
        case 4800:
            nBaud = B4800;
            break;
        case 9600:
            nBaud = B9600;
            break;
        case 19200:
            nBaud = B19200;
            break;
        case 38400:
            nBaud = B38400;
            break;
#ifndef  OS_UNIX_AIX//在AIX unix 中不支持B57600、B115200
        case 57600:
            nBaud = B57600;
            break;
        case 115200:
            nBaud = B115200;
            break;
#endif
        default:
            nBaud = B9600;
            break;
     }

    /*设置终端设备新属性*/
    stTermAttr = g_stOldTermAttr; 
    stTermAttr.c_iflag = 0;	
    stTermAttr.c_oflag = 0;	
    stTermAttr.c_cflag &=~CBAUD;    //波特率掩码 (4+1 位)
    stTermAttr.c_cflag =(nBaud|CS8|CLOCAL|CREAD);
    stTermAttr.c_lflag &=~(ISIG|ECHO|ICANON|NOFLSH);
    stTermAttr.c_iflag &=~(IXON|INLCR|IGNCR|ICRNL|ISTRIP);
    stTermAttr.c_oflag &=~OPOST;
    stTermAttr.c_cc[VMIN]=0;
    stTermAttr.c_cc[VTIME]=1;
    stTermAttr.c_cc[VQUIT]=0x7f;
    
    /*使属性生效*/
    nRetVal = ioctl( nFd, TCSETAF, &stTermAttr);
    if( nRetVal < 0 )
    {
        return RET_ERROR;
    }
    
    return RET_SUCCESS;
}



/**************************************************
Function: Star_GetOpenAuxCmd
Description: 获取实达辅口打开指令
Input: nAuxPort  -- 辅口号
           nBaud      -- 波特率
Output: pchAuxCmd  -- 获得的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int Star_GetOpenAuxCmd( const int nAuxPort, const int nBaud, char* pchAuxCmd )
{                  
    int i, n; 
    char c; 
    char cmd[ MAX_LEN ];                               
    
/*    根据辅口进行选择字符*/
    c = 'Y';
    for(i=0; i<sizeof(aux_ser)/sizeof(int); i++) 
    { 
        if(nAuxPort == aux_ser[i])
        {
            c = star_ser[i];
            break;
        }
    }
    
/*    设置波特率*/
    n = 0;
    for(i=0; i<sizeof(bp_pra)/sizeof(int); i++) 
    { 
        if(nBaud == bp_pra[i])
        {
            n = bp_ser[0][i];
            break;
        }
    }
        
/*    生成打开辅口指令*/
    memset( cmd, 0, sizeof( cmd ));
   // snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN, n, 0, 0, 0, c );
    sprintf( cmd, CMD_AUX_OPEN, n, 0, 0, 0, c );
    strcat( cmd, CMD_AUX_DTHOBO );      /*辅口收到一个字节数据马上发给服务器*/
    strcat( cmd, CMD_AUX_DATATOHOST );  /*辅口数据直接送主机*/
    strcat( cmd, CMD_AUX_ENABLE );      /*允许辅口*/  
    strcpy( pchAuxCmd, cmd );
    
    return RET_SUCCESS;
}
/**************************************************
Function:Star_GetClosAuxCmd
Description: 得到实达关闭辅口的指令
Input: none 
Output:pchAuxCmd-- 获取的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int Star_GetClosAuxCmd( char* pchAuxCmd )
{
    /*结束辅口发送数据，关闭辅口*/    
    //snprintf( pchAuxCmd, MAX_LEN - 1, "%s%s", CMD_AUX_END, CMD_AUX_DISABLE );
	sprintf( pchAuxCmd, "%s%s", CMD_AUX_END, CMD_AUX_DISABLE );
    return RET_SUCCESS;
}
/**************************************************
Function:GG_GetOpenAuxCmd
Description: 得到国光打开辅口的指令
Input: nAuxPort  -- 辅口号
           nBaud      -- 波特率
Output: pchAuxCmd  -- 获得的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int GG_GetOpenAuxCmd( const int nAuxPort, const int nBaud, char* pchAuxCmd )
{
    int c, n, i;
    char cmd[ MAX_LEN ];           /*指令*/
    
    /*根据辅口进行选择字符*/
    c = nAuxPort;
    if( nAuxPort > 4 || nAuxPort < 1 )
    {
        c = 1;
    }
    
    /*设置波特率*/
    n = 8;
    for(i=0; i<sizeof(bp_pra)/sizeof(int); i++) 
    { 
        if(nBaud == bp_pra[i])
        {
            n = bp_ser[1][i];
            break;
        }
    }
    
    /*生成打开辅口指令*/
    memset( cmd, 0, sizeof( cmd ));
    //snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN_GG, c, 1, n, 3, 1, 0 );  
    sprintf( cmd, CMD_AUX_OPEN_GG, c, 1, n, 3, 1, 0 ); 
    strcpy( pchAuxCmd, cmd );
    //debugPrintf("国光辅口打开指令%s\n", cmd );
    return RET_SUCCESS;
}
/**************************************************
Function:GG_GetCloseAuxCmd
Description:  得到国光关闭辅口的指令
Input: none 
Output:pchAuxCmd-- 获取的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int GG_GetCloseAuxCmd( char* pchAuxCmd )
{   
    /*结束辅口发送数据，关闭辅口  */  
    sprintf( pchAuxCmd, "%s", CMD_AUX_CLOSE_GG );
// snprintf( pchAuxCmd, MAX_LEN - 1, "%s", CMD_AUX_CLOSE_GG );
    return RET_SUCCESS;
}
/**************************************************
Function:GW_GetOpenAuxCmd
Description: 得到长城打开辅口的指令
Input: nAuxPort  -- 辅口号
           nBaud      -- 波特率
Output: pchAuxCmd  -- 获得的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int GW_GetOpenAuxCmd( const int nAuxPort, char* pchAuxCmd )
{
    char cmd[ MAX_LEN ];           /*指令*/
    int c;                          /*串口*/
    
    /*根据辅口进行选择字符*/
    c = nAuxPort;
    if( nAuxPort > 4 || nAuxPort < 1 )
    {
        c = 1;
    }
    
    /*生成打开辅口指令*/
    memset( cmd, 0, sizeof( cmd ));
    //snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN_GW, c - 1 );    
    sprintf( cmd, CMD_AUX_OPEN_GW, c - 1 );    
    strcpy( pchAuxCmd, cmd );
    
    return RET_SUCCESS;
}
/**************************************************
Function:GW_GetCloseAuxCmd
Description: 得到长城关闭辅口的指令
Input: none 
Output:pchAuxCmd-- 获取的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int GW_GetCloseAuxCmd( char* pchAuxCmd )
{  
    /*结束辅口发送数据，关闭辅口*/    
//    snprintf( pchAuxCmd, MAX_LEN - 1, "%s", CMD_AUX_CLOSE_GW );
    sprintf( pchAuxCmd,  "%s", CMD_AUX_CLOSE_GW );
    return RET_SUCCESS;
}
/**************************************************
Function:WYSE_GetOpenAuxCmd
Description: 得到WYSE打开辅口的指令
Input: nAuxPort  -- 辅口号
           nBaud      -- 波特率
Output: pchAuxCmd  -- 获得的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int WYSE_GetOpenAuxCmd( const int nAuxPort, char* pchAuxCmd )
{
    int c, i;
    char cmd[ MAX_LEN ];           /*指令*/
    
    /*根据辅口进行选择字符*/
    c = 45;
    for(i=0; i<sizeof(aux_ser)/sizeof(int); i++)
    { 
        if(nAuxPort == aux_ser[i])
        {
            c = wyse_ser[i];
            break;
        }
    }
    
    /*生成打开辅口指令*/
    memset( cmd, 0, sizeof( cmd ));
//    snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN_WYSE, c );
    sprintf( cmd,  CMD_AUX_OPEN_WYSE, c );
    strcat( cmd, CMD_AUX_ENABLE_WYSE );
    strcpy( pchAuxCmd, cmd );
    
    return RET_SUCCESS;
}
/**************************************************
Function:WYSE_GetCloseAuxCmd
Description: 得到WYSE关闭辅口的指令
Input: none 
Output:pchAuxCmd-- 获取的辅口指令
Return: RET_SUCCESS -- 成功
Others: 
*************************************************/
static int WYSE_GetCloseAuxCmd( char* pchAuxCmd )
{    
    /*结束辅口发送数据，关闭辅口*/    
    //snprintf( pchAuxCmd, MAX_LEN - 1, "%s", CMD_AUX_DISABLE_WYSE );
    sprintf( pchAuxCmd, "%s", CMD_AUX_DISABLE_WYSE );
    return RET_SUCCESS;
}


/**************************************************
Function:CT_OpenAuxPort  
Description: 辅口打开函数
Input:    nType  -- 仿真类型
            nAuxPort -- 辅口号
            nBaud -- 波特率
            nLockKeyboard  -- 是否锁键盘 0 -- 不锁，其余-- 锁键盘
Output: none
Return: RET_SUCCESS -- 成功  others -- 失败
Others: 
*************************************************/
int CT_OpenAuxPort( const int nType, const int nAuxPort, const int nBaud ,int nLockKeyboard )
{
    char cmd[ MAX_LEN ];
    
    /*验证参数*/
    if( nAuxPort < 1 )
    {
        return RET_INVALID_PARAM;
    }

    g_AuxOp.nSimType = nType;
    
    /*获得辅口指令和s_AUXLEAD， s_AUXEND命令设置*/
    memset( cmd, 0, sizeof( cmd ));
    memset( g_AuxOp.auxLead, 0, sizeof( g_AuxOp.auxLead ) );
    memset( g_AuxOp.auxEnd, 0, sizeof( g_AuxOp.auxEnd ) );  
    g_AuxOp.auxLeadLen = 0;
    g_AuxOp.auxEndLen  = 0;
    switch( nType )
    {
        case AUX_SIM_STAR:/*实达*/
            Star_GetOpenAuxCmd( nAuxPort, nBaud, cmd );
            strncpy( g_AuxOp.auxLead, CMD_AUX_LEAD, strlen( CMD_AUX_LEAD ) );
            strncpy( g_AuxOp.auxEnd, CMD_AUX_END, strlen( CMD_AUX_END ) );            
            break;          
        case AUX_SIM_GUOGUANG:/*国光*/
            GG_GetOpenAuxCmd( nAuxPort, nBaud, cmd );
            strncpy( g_AuxOp.auxLead, CMD_AUX_LEAD_GG, strlen( CMD_AUX_LEAD_GG ) );
            strncpy( g_AuxOp.auxEnd, CMD_AUX_END_GG, strlen( CMD_AUX_END_GG ) );          
            break;           
        case AUX_SIM_GREATWALL:/*长城*/
            GW_GetOpenAuxCmd( nAuxPort, cmd );
            break;
        case AUX_SIM_WYSE:/*WYSE*/
            WYSE_GetOpenAuxCmd( nAuxPort, cmd );
            break;
        default:
            Star_GetOpenAuxCmd( nAuxPort, nBaud, cmd );
            strncpy( g_AuxOp.auxLead, CMD_AUX_LEAD, strlen( CMD_AUX_LEAD ) );
            strncpy( g_AuxOp.auxEnd, CMD_AUX_END, strlen( CMD_AUX_END ) );            
            break;
     }
    g_AuxOp.auxLeadLen = strlen( g_AuxOp.auxLead );
    g_AuxOp.auxEndLen  = strlen( g_AuxOp.auxEnd );
    
    if( nLockKeyboard )
    {
        DbgPrintf("锁住键盘\n");
        CT_SendDataToTransPort( (U8*)CMD_LOCK_KEYBOARD, strlen( CMD_LOCK_KEYBOARD ));
    }

    /*发送打开辅口指令*/
    CT_SendDataToTransPort( (U8*)cmd, strlen( cmd ));
    
    if( AUX_SIM_GREATWALL == nType  )
    {
        return RET_SUCCESS;
    }    
    
    return RET_SUCCESS;
}
/**************************************************
Function:CT_CloseAuxPort
Description: 关闭辅口
Input: none
Output:none
Return: RET_SUCCESS -- 成功 Others -- 失败
Others: 
*************************************************/
int CT_CloseAuxPort( int nOpenKeyboard )
{
    char cmd[ MAX_LEN ];
    
    /*得到关闭辅口指令*/
    memset( cmd, 0, sizeof( cmd )); 
    switch(  g_AuxOp.nSimType )
    {
        case AUX_SIM_STAR:
            Star_GetClosAuxCmd( cmd );
            break;          
        case AUX_SIM_GUOGUANG:
            GG_GetCloseAuxCmd( cmd );
            break;
        case AUX_SIM_GREATWALL:
            GW_GetCloseAuxCmd( cmd );
            break;
        case AUX_SIM_WYSE:
            WYSE_GetCloseAuxCmd( cmd );
            break;
        default:
             g_AuxOp.nSimType = AUX_SIM_STAR;
            Star_GetClosAuxCmd( cmd );
            break;
     }  
    
    /*发送指令*/ 
    CT_SendDataToTransPort( (U8*)cmd, strlen( cmd ));
    
    /*解锁键盘*/
    if( nOpenKeyboard )
    {
         DbgPrintf("解锁键盘\n");
         CT_SendDataToTransPort( (U8*)CMD_UNCLOCK_KEYBOARD, strlen( CMD_UNCLOCK_KEYBOARD ));
    }

    return RET_SUCCESS;
}


/**************************************************
Function:ICC_OpenLinuxAux
Description: 打开终端
Input:  TermType      -终端类型
            PortNo       -辅口号
            nBaud       –波特率
Output: none 
Return: RET_SUCCESS -- 成功 Others -- 失败
Others: 要先打开串口，然后在打开终端辅口。
*************************************************/
S32 ICC_OpenLinux( char* szTtyName, const S32 nBaud )
{
	int nRetVal = RET_ERROR;

	if( g_AuxFd != INVALID_DESCRIPTOR )
	{
		close( g_AuxFd );
	}

	/*得到终端设备句柄*/
	g_AuxFd = open( szTtyName, O_RDWR );
	if( g_AuxFd < 0 )
	{
		g_AuxFd = INVALID_DESCRIPTOR;

		return RET_FAIL_OPEN_FILE;
	}

	/*设置终端属性*/
	if( RET_SUCCESS != SetTerm( g_AuxFd, nBaud ) )
	{
		close( g_AuxFd );
		g_AuxFd = INVALID_DESCRIPTOR;
		return RET_ERROR;
	}
					
	return RET_SUCCESS;
}

S32 ICC_OpenLinuxAux( const S32 nTermType,const S32 nAuxIndex, const S32 nBaud, const S32 nLockKeyboard )
{
    int nRetVal = RET_ERROR;

    char szTtyName[ MAX_LEN ] = {0};
    
    /*组织设备名*/
    memset(szTtyName, 0x0, sizeof(szTtyName));
    strcpy(szTtyName, (char*)ttyname(1));
    
    /*清除不可见字符*/
    ClearInvisibleCharInStr(szTtyName);
    DbgPrintf("LinuxOpenPort szTtyName =%s\r\n",szTtyName);

    nRetVal = ICC_OpenLinux( szTtyName, nBaud );
    if( RET_SUCCESS != nRetVal )
    {

        return nRetVal;
    }
    
    if( RET_SUCCESS ==  CT_OpenAuxPort( nTermType,nAuxIndex, nBaud, nLockKeyboard ) )
    {
       nRetVal = RET_SUCCESS;
    }
    else 
    {
        ResetAux();
        close( g_AuxFd );
        g_AuxFd = INVALID_DESCRIPTOR;
        nRetVal = RET_ERROR;
    }
                    
    return nRetVal;
}

/**************************************************
Function:ICC_CloseLinuxAux
Description: 关闭终端
Input: nLockKeyboard 是否锁键盘标识
Output:none
Return: RET_SUCCESS  成功
Others: 要先关闭终端辅口,后关闭串口
*************************************************/
S32 ICC_CloseLinux( void )
{   
	if( g_AuxFd != INVALID_DESCRIPTOR )
	{
		//关闭串口
		ResetAux();
		close( g_AuxFd );
		g_AuxFd = INVALID_DESCRIPTOR;
	}

	return RET_SUCCESS;
}

S32 ICC_CloseLinuxAux( const S32 nLockKeyboard )
{   
    if( g_AuxFd != INVALID_DESCRIPTOR )
    {
        /*关闭辅口*/
        CT_CloseAuxPort( nLockKeyboard );
        
        //关闭串口
        ICC_CloseLinux();
    }

    return RET_SUCCESS;
}

/**************************************************
Function:ICC_WriteLinuxAux
Description: 向辅口发送数据
Input: pInData -- 发送的数据
          nLen   --- 发送的数据长度
          
Output:none
Return: >0 -- 发送的数据长度  others -- 失败
Others: 
*************************************************/
S32 ICC_WriteLinux( U8* pInData, S32 nLen )

{
    int nWriteLen = 0;

    /*验证参数*/
    if( NULL == pInData || nLen < 1 )
    {
        return RET_INVALID_PARAM;
    }

    //向串口发送数据
    return CT_SendDataToTransPort((U8*)pInData, nLen );   

}

S32 ICC_WriteLinuxAux( U8* pInData, S32 nLen )

{
    int nWriteLen;

    /*验证参数*/
    if( NULL == pInData || nLen < 1 )
    {
        return RET_INVALID_PARAM;
    }
     
    //主机送到终端的数据全部送给辅口
    if(g_AuxOp.auxLeadLen)
    {
        CT_SendDataToTransPort( (U8*)g_AuxOp.auxLead, g_AuxOp.auxLeadLen);
    }

    //向串口发送数据
    nWriteLen = ICC_WriteLinux((U8*)pInData, nLen );   

    //主机结束向辅口送数据
    if(g_AuxOp.auxEndLen)
    {
        CT_SendDataToTransPort( (U8*)g_AuxOp.auxEnd, g_AuxOp.auxEndLen);
    }
    return nWriteLen;
}

S32 ReadBlock(S32 fd, U8 *pData, S32 nDataLen)
{
    return read(fd, pData, nDataLen);
}

S32 ReadUnBlock(S32 fd, U8 *pData, S32 nDataLen)
{
    fd_set input;

    struct timeval timeout;

    int n;

    timeout.tv_sec=1;

    timeout.tv_usec =0 ;

    DbgPrintf("ReadUnblock\n");

    FD_ZERO(&input); 

    FD_SET(fd,&input); 

    n = select(fd+1, &input, NULL, NULL,&timeout); 

    if (n <= 0) 
    {
        return 0;
    }

    else

    {    

        if (FD_ISSET(fd, &input)) 

        {

            return read (fd,pData,nDataLen);

        }

    }

    return 0;
}
/**************************************************
Function:ICC_ReadLinuxAux
Description: 从辅口取得数据
Input: nLen  -- 输出缓存的大小
          nTimeOut -- 超时时间
          chEnd     -- 结束字符
          
Output:pOutData -- 读取的数据
Return: >0 -- 读取到的数据长度， Others -- 失败
Others: 
*************************************************/
S32 ICC_ReadLinux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition )

{
    time_t stStartTime,stEndTime; 
    S32 nReadLen = 0;   /*读取的字节数*/
    S32 nTempLen = 0;                  /*临时变量*/
    S32 nRetVal = RET_SUCCESS;  /*返回值*/


    //return LinuxRecvData( szRecvBuf, nMaxLen, &pEnd, nTimeOut);
    
    if( ( INVALID_DESCRIPTOR == g_AuxFd ) || (NULL == szRecvBuf) )
    {
        return RET_INVALID_PARAM;
    }

    /*获取起始时间*/
    time( &stStartTime );  
  
    while( 1 )
    {
        /*读取串口数据*/
#ifdef OPER_AUX_UN_BLOCK
       nTempLen = ReadUnBlock( g_AuxFd, szRecvBuf+nReadLen, nMaxLen - nReadLen );           
#else
       nTempLen = ReadBlock( g_AuxFd, szRecvBuf+nReadLen, nMaxLen - nReadLen );   
#endif
        if( nTempLen < 0 )
        {
            nRetVal = RET_READ_ERROR;
            break;
        }
        nReadLen += nTempLen; 
        if( nMaxLen <= nReadLen )
        {
             nRetVal = nMaxLen;
             break;
        }  


        if( ( lpEndCondition )&&( !lpEndCondition( szRecvBuf, nReadLen ) ) )
        {
             nRetVal = nReadLen;
             break;
        }



        /*!<time out */
        time( &stEndTime );
        if( stEndTime - stStartTime > nTimeOut )
        {
            nRetVal = RET_FAIL_TIMEOUT;
            break;
        }
    }
    
    return nRetVal;
}

S32 ICC_ReadLinuxAux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition )

{
    return ICC_ReadLinux( szRecvBuf, nMaxLen, nTimeOut, lpEndCondition );
}



