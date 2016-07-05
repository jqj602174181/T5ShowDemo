/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:     pcsc.c
*
*  Author:      laiwenhui
*
*  Date:        2012-10-12 09:59
*
*  Description: window pc/sc通信协议标准
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "GlobalDef.h"
#include "StrOp.h"
#include  "Debug.h"
#include <WinSCard.h>
#include "pcsc.h"


#pragma comment(lib, "winscard.lib")

static SCARDCONTEXT  s_hSC;//资源管理器句柄
static SCARDHANDLE   s_hCardHandle;//卡片句柄 

static  int s_RecvLen = 0;
static unsigned char s_RecvBuf[1024];

int PCSC_GetPacketBody(U8 head,U8 end,U8 * szInData, int nInDataLen,U8* szOutData)
{   
	int nLen = -1;
	int nBeginPos = 0;
	int nEndPos = 0;
	//寻找头起始位置
	while( nInDataLen > nBeginPos )
	{

		if( head == szInData[nBeginPos++] )
		{

			break;
		}   
	}

	if( nBeginPos >= nInDataLen-1 )
	{        
		return -1;   
	}
	nEndPos = nBeginPos;
	while(  nInDataLen >  nEndPos )
	{

		if(  end == szInData[nEndPos] )
		{
			break;
		}
		nEndPos ++;
	}
	nLen = nEndPos - nBeginPos;
	if((nLen > 0)&&( nEndPos<=(nInDataLen-1)) )
	{
		memcpy( szOutData, szInData+nBeginPos, nLen );
		return nLen;   
	}

	return -1;

}
/*
函数原型：int GetCardReaderIns( unsigned char * ucBuf, int iSize, unsigned char *szPacket )
功能描述： 
返回值： 
参数ComID含义：
*/
static int GetCardReaderIns( unsigned char * ucBuf, int iSize, unsigned char *szPacket )
{
	unsigned char ucPacket[ PACKET_LEN ];
	unsigned char ucTemp[ PACKET_LEN ];
	int iLen = 0;
    
    if( ( NULL == ucBuf ) || ( iSize <= 0 )||( NULL == szPacket ) )
    {
        return RET_INVALID_PARAM;    
    }
    
	memset( ucPacket, 0x0, sizeof(ucPacket) );
	memset( ucTemp, 0x0, sizeof(ucTemp) );
    iSize +=2;//增加一个字节标识成功
	//数据包长度
    ucTemp[iLen++] = ( iSize >> 8 )&0xFF;
    ucTemp[iLen++] =  ( iSize )&0xFF;
	ucTemp[iLen++] =  0x00;//表示成功
	ucTemp[iLen++] =  0x00;//表示成功
    //数据包
	memcpy( ucTemp+iLen, ucBuf, iSize);
	iLen +=iSize;
	//数据包LRC校验值,为data部分
    ucTemp[ iLen++ ] =  GetDataLRC( ucBuf , iSize );

    //组装数据
    ucPacket[0] = 0x02;//包头
	//数据拆分
	SplitData( ucTemp, iLen, ucPacket + 1 );
	iLen = iLen*2 + 1;
	//数据包尾
	ucPacket[ iLen++ ] = 0x03;
	
    memcpy( szPacket, ucPacket, iLen);
    return iLen;
}
/*
函数原型：int ReadCardReaderData( unsigned char * ucBuf, int  *pRecvLen )
功能描述：读读卡器数据。 
返回值：操作成功返回大于零的读卡器执行状态，否则返回负数状态码。
参数ComID含义： ucBuf -- 读取的除状态码外的数据 
                            pRecvLen -- ucBuf中的数据长度
*/
static int GetCardReaderData( unsigned char * szInsBuf,int nInLen, unsigned char * ucBuf, int  *pRecvLen )
{
	 int iLen = 0;
	 unsigned char ucPacket[ PACKET_LEN ];
	 unsigned char ucTemp[ PACKET_LEN ];

     if( ( NULL == szInsBuf ) || ( NULL == ucBuf )||( NULL == pRecvLen ) )
     {
         return RET_INVALID_PARAM;    
     }
    
	 memset( ucPacket, 0x0, sizeof(ucPacket) );
	 memset( ucTemp, 0x0, sizeof(ucTemp) );
	 memcpy( ucTemp,szInsBuf,24);
     iLen = PCSC_GetPacketBody( 0x02,0x03,szInsBuf, nInLen, ucPacket);
     
	 if( ( iLen< 2 )||( iLen%2 ) )//拆分数据至少也要大于2，且是偶数吧
	 {
		 return -1;
	 }
	 
	 //数据合并得到ss:数据单元长度Data_len(2个字节)+需传输的数据单元Data(不定长)+冗余检验值LRC(1个字节)
	 memset( ucTemp, 0x0, sizeof(ucTemp) );
	 MergeData( ucPacket, iLen ,ucTemp );
	 iLen /=2;  //合并后的数据长度
	 //比较校验值
	 if(  GetDataLRC( ucTemp + 2, iLen - 3 ) == ucTemp[ iLen - 1 ] )
	 {
		 //Data的实际长度Data_len,为16进制。
		 iLen = ucTemp[0]*256+ucTemp[1];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		 //Data的参数部分数据
		 *pRecvLen = iLen - 3;
		 memcpy( ucBuf,  ucTemp+5, iLen - 3);
		 return RET_SUCCESS;//返回状态码
	 }
	  return -1;
}

/**************************************************
Function: PcscOpenPort 
Description: 打开终端串口
Input: pTtyName -- 终端名
         nBaud       -- 波特率
Output: none
Return: ERR_SUCCESS -- 成功，other -- 失败
Others: 如果波特率nBaud 为非法值，则默认为9600
*************************************************/
int PcscOpenPort( char * pszCardReader )
{
    LONG lRetVal = 0;  
	int nRet = 0;
    DWORD dwListLen = 0;
    DWORD dwAP = 0;
    char mszReaderList[1024];

    //建立资源管理器的上下文
    lRetVal = SCardEstablishContext( SCARD_SCOPE_USER, NULL, NULL, &s_hSC ); 
    if ( SCARD_S_SUCCESS == lRetVal ) 
    {
        //获得系统中安装的读卡器列表
        memset( mszReaderList, 0x0, sizeof(mszReaderList) );
        dwListLen =  sizeof(mszReaderList);
        lRetVal = SCardListReaders(s_hSC, NULL, (LPTSTR)mszReaderList, &dwListLen); 
		//printf(" lRetVal = %d %s\n", lRetVal, mszReaderList );
        if ( SCARD_S_SUCCESS == lRetVal ) 
        {
            //与读卡器（智能卡）连接,连接第一个读卡器上的卡片
            lRetVal = SCardConnect( s_hSC, (LPCWSTR)mszReaderList, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &s_hCardHandle, &dwAP ); 
			//printf(" SCardConnect lRetVal = %X \n", lRetVal );
            if ( SCARD_S_SUCCESS == lRetVal ) 
            {
                nRet = RET_SUCCESS;//成功
            }
            else
            { 
                nRet =  -104;//"读卡器内无卡
            }
        }
        else
        {   
            nRet =  -101;
            
        }
    }
    else
    {       
        nRet = -101;
    }

    if( (RET_SUCCESS != nRet ) && ( 0 != s_hSC ) )
    {
        SCardReleaseContext(s_hSC);
        s_hSC = 0;
    }
	//printf("nRet=%d\n", nRet);
    return nRet;    
    
}
/**************************************************
Function:PcscClosePort
Description: 关闭传输口
Input: none
Output:none
Return: ERR_SUCCESS  -- 成功
Others: 
*************************************************/
int PcscClosePort( void )
{
	 LONG lRetVal = 0;  
	 int nRet = RET_SUCCESS;
	 
	 // Power down the card on close
	 if( 0 != s_hCardHandle )
	 {     
	       //断开连接
	       lRetVal = SCardDisconnect( s_hCardHandle, SCARD_UNPOWER_CARD ); 
		   if ( SCARD_S_SUCCESS == lRetVal ) 
		   {
		       //释放资源
			   SCardReleaseContext(s_hSC);
			   s_hCardHandle = 0;
			   s_hSC = 0;
			   nRet =   RET_SUCCESS;//成功
		   }
		   else	
		   {
			   nRet = -113;//SB_LN_ERR_CLOSE_PORT;
		   }
	 }
	 else
	 {
		 nRet = -102;//SB_LN_ERR_NO_CONNECT;
	 }
	 return nRet;    


}
/**************************************************
Function: PcscSendData
Description: 发送数据
Input: data  --  发送的数据
          len  数据长度
Output:none 
Return: 大于0 -- 成功 
Others: 
*************************************************/
int PcscSendData( const unsigned char *pSendData, const int nLen )
{
    int nSendLen = nLen;
	int a = 0;
    unsigned char szBuf[ 512 ];

    if( 0 != s_hCardHandle )
    { 
       memset( szBuf, 0x0, sizeof(szBuf));
       //获取数据包中的访问COS的数据
	   memcpy(szBuf, pSendData+3, nLen);

       memset( s_RecvBuf, 0x0, sizeof(s_RecvBuf) );
       s_RecvLen = sizeof(s_RecvBuf);
	   if( szBuf[0] == 0x00 && szBuf[1] == 0xA4 )
	   {
			nSendLen = szBuf[4] + 5;
	   }
	   else if( szBuf[0] == 0x80 && szBuf[1] == 0xA8 )
	   {
		    nSendLen = nLen -4;
	   }
	   else
	   {
		    nSendLen = nLen -3;
	   }
	   a = SCardTransmit( s_hCardHandle, SCARD_PCI_T0, pSendData+3, nSendLen, NULL, s_RecvBuf, &s_RecvLen );
       if( SCARD_S_SUCCESS == a )
       {
           return nLen;//这里返回发送的原始数据长度
       }
    }
    return -1;
}
/**************************************************
Function: PcscRecvData
Description: 从终端接收数据
Input: data  --  发送的数据
          len  数据长度
Output:none 
Return: 大于0 -- 读取的数据长度 
Others: 
*************************************************/
int PcscRecvData( unsigned char * szRecvBuf, int nMaxLen)
{ 
    int nLen = 0;
    //读数据进行判断
    nLen = nMaxLen > s_RecvLen ? s_RecvLen: nMaxLen ;
    s_RecvLen = 0;//设置为零
    return GetCardReaderIns( s_RecvBuf, nLen, szRecvBuf );   
    
}
