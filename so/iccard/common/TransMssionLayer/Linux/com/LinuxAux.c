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

struct termio g_stOldTermAttr;//����ȫ�ֵ��ն�����



typedef struct AuxOp_t
{
     S32   nSimType; //�������� 
     S8  auxLead[ MAX_LEN ];//��ʼд���ݱ�ʶ 
     S32   auxLeadLen;
     S8  auxEnd[ MAX_LEN];   //����д���ݱ�ʶ
     S32   auxEndLen;
    
}AuxOp;


static int bp_pra[] = { 9600, 4800, 2400, 1200, 600, 300, 19200, 38400, 115200 }; /*�����ʲ���*/ 
static int bp_ser[][ 9 ] = {{0, 1, 2, 3, 5, 6, 10, 11, 12}, {8, 7, 6, 5, 4, 3, 9, 8, 8}};             /*�����ʲ������*/ 
static int aux_ser[] = {AUX1, AUX2, AUX3, AUX4};                                                /*���ڱ�ʶ���*/ 
static int wyse_ser[] = {45, 54, 60, 45};                                            /*WYSE�ĸ��ڱ�ʶ���*/ 
static char star_ser[] = {'Y', 'Z', 'X', 'V'};                                                                 /*ʵ�︨�ڱ�ʶ���*/

/*���ڸ��ڲ����漰��һЩȫ�ֱ������������ͳһ����*/
AuxOp g_AuxOp;


static S32 g_AuxFd = INVALID_DESCRIPTOR;//����ȫ�ֵ��ļ���ʶ��

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

    tv_timeout.tv_sec =1;/*��ʱʱ��3��*/

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

    /*��֤����*/
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
      
      /*  ����Ϊ����ֱ�ӷ���*/
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
    /*��֤����*/
    if( INVALID_DESCRIPTOR  == g_AuxFd )
    {
        return RET_INVALID_PARAM;
    }

    /*�ָ��ն�ԭʼ����*/
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
   
    /*��֤����*/
    if( INVALID_DESCRIPTOR == nFd )
    {
        return RET_INVALID_PARAM;
    }
    
    /*���ԭʼ���ն˲���*/
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
#ifndef  OS_UNIX_AIX//��AIX unix �в�֧��B57600��B115200
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

    /*�����ն��豸������*/
    stTermAttr = g_stOldTermAttr; 
    stTermAttr.c_iflag = 0;	
    stTermAttr.c_oflag = 0;	
    stTermAttr.c_cflag &=~CBAUD;    //���������� (4+1 λ)
    stTermAttr.c_cflag =(nBaud|CS8|CLOCAL|CREAD);
    stTermAttr.c_lflag &=~(ISIG|ECHO|ICANON|NOFLSH);
    stTermAttr.c_iflag &=~(IXON|INLCR|IGNCR|ICRNL|ISTRIP);
    stTermAttr.c_oflag &=~OPOST;
    stTermAttr.c_cc[VMIN]=0;
    stTermAttr.c_cc[VTIME]=1;
    stTermAttr.c_cc[VQUIT]=0x7f;
    
    /*ʹ������Ч*/
    nRetVal = ioctl( nFd, TCSETAF, &stTermAttr);
    if( nRetVal < 0 )
    {
        return RET_ERROR;
    }
    
    return RET_SUCCESS;
}



/**************************************************
Function: Star_GetOpenAuxCmd
Description: ��ȡʵ�︨�ڴ�ָ��
Input: nAuxPort  -- ���ں�
           nBaud      -- ������
Output: pchAuxCmd  -- ��õĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int Star_GetOpenAuxCmd( const int nAuxPort, const int nBaud, char* pchAuxCmd )
{                  
    int i, n; 
    char c; 
    char cmd[ MAX_LEN ];                               
    
/*    ���ݸ��ڽ���ѡ���ַ�*/
    c = 'Y';
    for(i=0; i<sizeof(aux_ser)/sizeof(int); i++) 
    { 
        if(nAuxPort == aux_ser[i])
        {
            c = star_ser[i];
            break;
        }
    }
    
/*    ���ò�����*/
    n = 0;
    for(i=0; i<sizeof(bp_pra)/sizeof(int); i++) 
    { 
        if(nBaud == bp_pra[i])
        {
            n = bp_ser[0][i];
            break;
        }
    }
        
/*    ���ɴ򿪸���ָ��*/
    memset( cmd, 0, sizeof( cmd ));
   // snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN, n, 0, 0, 0, c );
    sprintf( cmd, CMD_AUX_OPEN, n, 0, 0, 0, c );
    strcat( cmd, CMD_AUX_DTHOBO );      /*�����յ�һ���ֽ��������Ϸ���������*/
    strcat( cmd, CMD_AUX_DATATOHOST );  /*��������ֱ��������*/
    strcat( cmd, CMD_AUX_ENABLE );      /*������*/  
    strcpy( pchAuxCmd, cmd );
    
    return RET_SUCCESS;
}
/**************************************************
Function:Star_GetClosAuxCmd
Description: �õ�ʵ��رո��ڵ�ָ��
Input: none 
Output:pchAuxCmd-- ��ȡ�ĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int Star_GetClosAuxCmd( char* pchAuxCmd )
{
    /*�������ڷ������ݣ��رո���*/    
    //snprintf( pchAuxCmd, MAX_LEN - 1, "%s%s", CMD_AUX_END, CMD_AUX_DISABLE );
	sprintf( pchAuxCmd, "%s%s", CMD_AUX_END, CMD_AUX_DISABLE );
    return RET_SUCCESS;
}
/**************************************************
Function:GG_GetOpenAuxCmd
Description: �õ�����򿪸��ڵ�ָ��
Input: nAuxPort  -- ���ں�
           nBaud      -- ������
Output: pchAuxCmd  -- ��õĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int GG_GetOpenAuxCmd( const int nAuxPort, const int nBaud, char* pchAuxCmd )
{
    int c, n, i;
    char cmd[ MAX_LEN ];           /*ָ��*/
    
    /*���ݸ��ڽ���ѡ���ַ�*/
    c = nAuxPort;
    if( nAuxPort > 4 || nAuxPort < 1 )
    {
        c = 1;
    }
    
    /*���ò�����*/
    n = 8;
    for(i=0; i<sizeof(bp_pra)/sizeof(int); i++) 
    { 
        if(nBaud == bp_pra[i])
        {
            n = bp_ser[1][i];
            break;
        }
    }
    
    /*���ɴ򿪸���ָ��*/
    memset( cmd, 0, sizeof( cmd ));
    //snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN_GG, c, 1, n, 3, 1, 0 );  
    sprintf( cmd, CMD_AUX_OPEN_GG, c, 1, n, 3, 1, 0 ); 
    strcpy( pchAuxCmd, cmd );
    //debugPrintf("���⸨�ڴ�ָ��%s\n", cmd );
    return RET_SUCCESS;
}
/**************************************************
Function:GG_GetCloseAuxCmd
Description:  �õ�����رո��ڵ�ָ��
Input: none 
Output:pchAuxCmd-- ��ȡ�ĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int GG_GetCloseAuxCmd( char* pchAuxCmd )
{   
    /*�������ڷ������ݣ��رո���  */  
    sprintf( pchAuxCmd, "%s", CMD_AUX_CLOSE_GG );
// snprintf( pchAuxCmd, MAX_LEN - 1, "%s", CMD_AUX_CLOSE_GG );
    return RET_SUCCESS;
}
/**************************************************
Function:GW_GetOpenAuxCmd
Description: �õ����Ǵ򿪸��ڵ�ָ��
Input: nAuxPort  -- ���ں�
           nBaud      -- ������
Output: pchAuxCmd  -- ��õĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int GW_GetOpenAuxCmd( const int nAuxPort, char* pchAuxCmd )
{
    char cmd[ MAX_LEN ];           /*ָ��*/
    int c;                          /*����*/
    
    /*���ݸ��ڽ���ѡ���ַ�*/
    c = nAuxPort;
    if( nAuxPort > 4 || nAuxPort < 1 )
    {
        c = 1;
    }
    
    /*���ɴ򿪸���ָ��*/
    memset( cmd, 0, sizeof( cmd ));
    //snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN_GW, c - 1 );    
    sprintf( cmd, CMD_AUX_OPEN_GW, c - 1 );    
    strcpy( pchAuxCmd, cmd );
    
    return RET_SUCCESS;
}
/**************************************************
Function:GW_GetCloseAuxCmd
Description: �õ����ǹرո��ڵ�ָ��
Input: none 
Output:pchAuxCmd-- ��ȡ�ĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int GW_GetCloseAuxCmd( char* pchAuxCmd )
{  
    /*�������ڷ������ݣ��رո���*/    
//    snprintf( pchAuxCmd, MAX_LEN - 1, "%s", CMD_AUX_CLOSE_GW );
    sprintf( pchAuxCmd,  "%s", CMD_AUX_CLOSE_GW );
    return RET_SUCCESS;
}
/**************************************************
Function:WYSE_GetOpenAuxCmd
Description: �õ�WYSE�򿪸��ڵ�ָ��
Input: nAuxPort  -- ���ں�
           nBaud      -- ������
Output: pchAuxCmd  -- ��õĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int WYSE_GetOpenAuxCmd( const int nAuxPort, char* pchAuxCmd )
{
    int c, i;
    char cmd[ MAX_LEN ];           /*ָ��*/
    
    /*���ݸ��ڽ���ѡ���ַ�*/
    c = 45;
    for(i=0; i<sizeof(aux_ser)/sizeof(int); i++)
    { 
        if(nAuxPort == aux_ser[i])
        {
            c = wyse_ser[i];
            break;
        }
    }
    
    /*���ɴ򿪸���ָ��*/
    memset( cmd, 0, sizeof( cmd ));
//    snprintf( cmd, sizeof( cmd ) - 1, CMD_AUX_OPEN_WYSE, c );
    sprintf( cmd,  CMD_AUX_OPEN_WYSE, c );
    strcat( cmd, CMD_AUX_ENABLE_WYSE );
    strcpy( pchAuxCmd, cmd );
    
    return RET_SUCCESS;
}
/**************************************************
Function:WYSE_GetCloseAuxCmd
Description: �õ�WYSE�رո��ڵ�ָ��
Input: none 
Output:pchAuxCmd-- ��ȡ�ĸ���ָ��
Return: RET_SUCCESS -- �ɹ�
Others: 
*************************************************/
static int WYSE_GetCloseAuxCmd( char* pchAuxCmd )
{    
    /*�������ڷ������ݣ��رո���*/    
    //snprintf( pchAuxCmd, MAX_LEN - 1, "%s", CMD_AUX_DISABLE_WYSE );
    sprintf( pchAuxCmd, "%s", CMD_AUX_DISABLE_WYSE );
    return RET_SUCCESS;
}


/**************************************************
Function:CT_OpenAuxPort  
Description: ���ڴ򿪺���
Input:    nType  -- ��������
            nAuxPort -- ���ں�
            nBaud -- ������
            nLockKeyboard  -- �Ƿ������� 0 -- ����������-- ������
Output: none
Return: RET_SUCCESS -- �ɹ�  others -- ʧ��
Others: 
*************************************************/
int CT_OpenAuxPort( const int nType, const int nAuxPort, const int nBaud ,int nLockKeyboard )
{
    char cmd[ MAX_LEN ];
    
    /*��֤����*/
    if( nAuxPort < 1 )
    {
        return RET_INVALID_PARAM;
    }

    g_AuxOp.nSimType = nType;
    
    /*��ø���ָ���s_AUXLEAD�� s_AUXEND��������*/
    memset( cmd, 0, sizeof( cmd ));
    memset( g_AuxOp.auxLead, 0, sizeof( g_AuxOp.auxLead ) );
    memset( g_AuxOp.auxEnd, 0, sizeof( g_AuxOp.auxEnd ) );  
    g_AuxOp.auxLeadLen = 0;
    g_AuxOp.auxEndLen  = 0;
    switch( nType )
    {
        case AUX_SIM_STAR:/*ʵ��*/
            Star_GetOpenAuxCmd( nAuxPort, nBaud, cmd );
            strncpy( g_AuxOp.auxLead, CMD_AUX_LEAD, strlen( CMD_AUX_LEAD ) );
            strncpy( g_AuxOp.auxEnd, CMD_AUX_END, strlen( CMD_AUX_END ) );            
            break;          
        case AUX_SIM_GUOGUANG:/*����*/
            GG_GetOpenAuxCmd( nAuxPort, nBaud, cmd );
            strncpy( g_AuxOp.auxLead, CMD_AUX_LEAD_GG, strlen( CMD_AUX_LEAD_GG ) );
            strncpy( g_AuxOp.auxEnd, CMD_AUX_END_GG, strlen( CMD_AUX_END_GG ) );          
            break;           
        case AUX_SIM_GREATWALL:/*����*/
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
        DbgPrintf("��ס����\n");
        CT_SendDataToTransPort( (U8*)CMD_LOCK_KEYBOARD, strlen( CMD_LOCK_KEYBOARD ));
    }

    /*���ʹ򿪸���ָ��*/
    CT_SendDataToTransPort( (U8*)cmd, strlen( cmd ));
    
    if( AUX_SIM_GREATWALL == nType  )
    {
        return RET_SUCCESS;
    }    
    
    return RET_SUCCESS;
}
/**************************************************
Function:CT_CloseAuxPort
Description: �رո���
Input: none
Output:none
Return: RET_SUCCESS -- �ɹ� Others -- ʧ��
Others: 
*************************************************/
int CT_CloseAuxPort( int nOpenKeyboard )
{
    char cmd[ MAX_LEN ];
    
    /*�õ��رո���ָ��*/
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
    
    /*����ָ��*/ 
    CT_SendDataToTransPort( (U8*)cmd, strlen( cmd ));
    
    /*��������*/
    if( nOpenKeyboard )
    {
         DbgPrintf("��������\n");
         CT_SendDataToTransPort( (U8*)CMD_UNCLOCK_KEYBOARD, strlen( CMD_UNCLOCK_KEYBOARD ));
    }

    return RET_SUCCESS;
}


/**************************************************
Function:ICC_OpenLinuxAux
Description: ���ն�
Input:  TermType      -�ն�����
            PortNo       -���ں�
            nBaud       �C������
Output: none 
Return: RET_SUCCESS -- �ɹ� Others -- ʧ��
Others: Ҫ�ȴ򿪴��ڣ�Ȼ���ڴ��ն˸��ڡ�
*************************************************/
S32 ICC_OpenLinux( char* szTtyName, const S32 nBaud )
{
	int nRetVal = RET_ERROR;

	if( g_AuxFd != INVALID_DESCRIPTOR )
	{
		close( g_AuxFd );
	}

	/*�õ��ն��豸���*/
	g_AuxFd = open( szTtyName, O_RDWR );
	if( g_AuxFd < 0 )
	{
		g_AuxFd = INVALID_DESCRIPTOR;

		return RET_FAIL_OPEN_FILE;
	}

	/*�����ն�����*/
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
    
    /*��֯�豸��*/
    memset(szTtyName, 0x0, sizeof(szTtyName));
    strcpy(szTtyName, (char*)ttyname(1));
    
    /*������ɼ��ַ�*/
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
Description: �ر��ն�
Input: nLockKeyboard �Ƿ������̱�ʶ
Output:none
Return: RET_SUCCESS  �ɹ�
Others: Ҫ�ȹر��ն˸���,��رմ���
*************************************************/
S32 ICC_CloseLinux( void )
{   
	if( g_AuxFd != INVALID_DESCRIPTOR )
	{
		//�رմ���
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
        /*�رո���*/
        CT_CloseAuxPort( nLockKeyboard );
        
        //�رմ���
        ICC_CloseLinux();
    }

    return RET_SUCCESS;
}

/**************************************************
Function:ICC_WriteLinuxAux
Description: �򸨿ڷ�������
Input: pInData -- ���͵�����
          nLen   --- ���͵����ݳ���
          
Output:none
Return: >0 -- ���͵����ݳ���  others -- ʧ��
Others: 
*************************************************/
S32 ICC_WriteLinux( U8* pInData, S32 nLen )

{
    int nWriteLen = 0;

    /*��֤����*/
    if( NULL == pInData || nLen < 1 )
    {
        return RET_INVALID_PARAM;
    }

    //�򴮿ڷ�������
    return CT_SendDataToTransPort((U8*)pInData, nLen );   

}

S32 ICC_WriteLinuxAux( U8* pInData, S32 nLen )

{
    int nWriteLen;

    /*��֤����*/
    if( NULL == pInData || nLen < 1 )
    {
        return RET_INVALID_PARAM;
    }
     
    //�����͵��ն˵�����ȫ���͸�����
    if(g_AuxOp.auxLeadLen)
    {
        CT_SendDataToTransPort( (U8*)g_AuxOp.auxLead, g_AuxOp.auxLeadLen);
    }

    //�򴮿ڷ�������
    nWriteLen = ICC_WriteLinux((U8*)pInData, nLen );   

    //���������򸨿�������
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
Description: �Ӹ���ȡ������
Input: nLen  -- �������Ĵ�С
          nTimeOut -- ��ʱʱ��
          chEnd     -- �����ַ�
          
Output:pOutData -- ��ȡ������
Return: >0 -- ��ȡ�������ݳ��ȣ� Others -- ʧ��
Others: 
*************************************************/
S32 ICC_ReadLinux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition )

{
    time_t stStartTime,stEndTime; 
    S32 nReadLen = 0;   /*��ȡ���ֽ���*/
    S32 nTempLen = 0;                  /*��ʱ����*/
    S32 nRetVal = RET_SUCCESS;  /*����ֵ*/


    //return LinuxRecvData( szRecvBuf, nMaxLen, &pEnd, nTimeOut);
    
    if( ( INVALID_DESCRIPTOR == g_AuxFd ) || (NULL == szRecvBuf) )
    {
        return RET_INVALID_PARAM;
    }

    /*��ȡ��ʼʱ��*/
    time( &stStartTime );  
  
    while( 1 )
    {
        /*��ȡ��������*/
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



