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
*  Description: window pc/scͨ��Э���׼
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

static SCARDCONTEXT  s_hSC;//��Դ���������
static SCARDHANDLE   s_hCardHandle;//��Ƭ��� 

static  int s_RecvLen = 0;
static unsigned char s_RecvBuf[1024];

int PCSC_GetPacketBody(U8 head,U8 end,U8 * szInData, int nInDataLen,U8* szOutData)
{   
	int nLen = -1;
	int nBeginPos = 0;
	int nEndPos = 0;
	//Ѱ��ͷ��ʼλ��
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
����ԭ�ͣ�int GetCardReaderIns( unsigned char * ucBuf, int iSize, unsigned char *szPacket )
���������� 
����ֵ�� 
����ComID���壺
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
    iSize +=2;//����һ���ֽڱ�ʶ�ɹ�
	//���ݰ�����
    ucTemp[iLen++] = ( iSize >> 8 )&0xFF;
    ucTemp[iLen++] =  ( iSize )&0xFF;
	ucTemp[iLen++] =  0x00;//��ʾ�ɹ�
	ucTemp[iLen++] =  0x00;//��ʾ�ɹ�
    //���ݰ�
	memcpy( ucTemp+iLen, ucBuf, iSize);
	iLen +=iSize;
	//���ݰ�LRCУ��ֵ,Ϊdata����
    ucTemp[ iLen++ ] =  GetDataLRC( ucBuf , iSize );

    //��װ����
    ucPacket[0] = 0x02;//��ͷ
	//���ݲ��
	SplitData( ucTemp, iLen, ucPacket + 1 );
	iLen = iLen*2 + 1;
	//���ݰ�β
	ucPacket[ iLen++ ] = 0x03;
	
    memcpy( szPacket, ucPacket, iLen);
    return iLen;
}
/*
����ԭ�ͣ�int ReadCardReaderData( unsigned char * ucBuf, int  *pRecvLen )
���������������������ݡ� 
����ֵ�������ɹ����ش�����Ķ�����ִ��״̬�����򷵻ظ���״̬�롣
����ComID���壺 ucBuf -- ��ȡ�ĳ�״̬��������� 
                            pRecvLen -- ucBuf�е����ݳ���
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
     
	 if( ( iLen< 2 )||( iLen%2 ) )//�����������ҲҪ����2������ż����
	 {
		 return -1;
	 }
	 
	 //���ݺϲ��õ�ss:���ݵ�Ԫ����Data_len(2���ֽ�)+�贫������ݵ�ԪData(������)+�������ֵLRC(1���ֽ�)
	 memset( ucTemp, 0x0, sizeof(ucTemp) );
	 MergeData( ucPacket, iLen ,ucTemp );
	 iLen /=2;  //�ϲ�������ݳ���
	 //�Ƚ�У��ֵ
	 if(  GetDataLRC( ucTemp + 2, iLen - 3 ) == ucTemp[ iLen - 1 ] )
	 {
		 //Data��ʵ�ʳ���Data_len,Ϊ16���ơ�
		 iLen = ucTemp[0]*256+ucTemp[1];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
		 //Data�Ĳ�����������
		 *pRecvLen = iLen - 3;
		 memcpy( ucBuf,  ucTemp+5, iLen - 3);
		 return RET_SUCCESS;//����״̬��
	 }
	  return -1;
}

/**************************************************
Function: PcscOpenPort 
Description: ���ն˴���
Input: pTtyName -- �ն���
         nBaud       -- ������
Output: none
Return: ERR_SUCCESS -- �ɹ���other -- ʧ��
Others: ���������nBaud Ϊ�Ƿ�ֵ����Ĭ��Ϊ9600
*************************************************/
int PcscOpenPort( char * pszCardReader )
{
    LONG lRetVal = 0;  
	int nRet = 0;
    DWORD dwListLen = 0;
    DWORD dwAP = 0;
    char mszReaderList[1024];

    //������Դ��������������
    lRetVal = SCardEstablishContext( SCARD_SCOPE_USER, NULL, NULL, &s_hSC ); 
    if ( SCARD_S_SUCCESS == lRetVal ) 
    {
        //���ϵͳ�а�װ�Ķ������б�
        memset( mszReaderList, 0x0, sizeof(mszReaderList) );
        dwListLen =  sizeof(mszReaderList);
        lRetVal = SCardListReaders(s_hSC, NULL, (LPTSTR)mszReaderList, &dwListLen); 
		//printf(" lRetVal = %d %s\n", lRetVal, mszReaderList );
        if ( SCARD_S_SUCCESS == lRetVal ) 
        {
            //������������ܿ�������,���ӵ�һ���������ϵĿ�Ƭ
            lRetVal = SCardConnect( s_hSC, (LPCWSTR)mszReaderList, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &s_hCardHandle, &dwAP ); 
			//printf(" SCardConnect lRetVal = %X \n", lRetVal );
            if ( SCARD_S_SUCCESS == lRetVal ) 
            {
                nRet = RET_SUCCESS;//�ɹ�
            }
            else
            { 
                nRet =  -104;//"���������޿�
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
Description: �رմ����
Input: none
Output:none
Return: ERR_SUCCESS  -- �ɹ�
Others: 
*************************************************/
int PcscClosePort( void )
{
	 LONG lRetVal = 0;  
	 int nRet = RET_SUCCESS;
	 
	 // Power down the card on close
	 if( 0 != s_hCardHandle )
	 {     
	       //�Ͽ�����
	       lRetVal = SCardDisconnect( s_hCardHandle, SCARD_UNPOWER_CARD ); 
		   if ( SCARD_S_SUCCESS == lRetVal ) 
		   {
		       //�ͷ���Դ
			   SCardReleaseContext(s_hSC);
			   s_hCardHandle = 0;
			   s_hSC = 0;
			   nRet =   RET_SUCCESS;//�ɹ�
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
Description: ��������
Input: data  --  ���͵�����
          len  ���ݳ���
Output:none 
Return: ����0 -- �ɹ� 
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
       //��ȡ���ݰ��еķ���COS������
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
           return nLen;//���ﷵ�ط��͵�ԭʼ���ݳ���
       }
    }
    return -1;
}
/**************************************************
Function: PcscRecvData
Description: ���ն˽�������
Input: data  --  ���͵�����
          len  ���ݳ���
Output:none 
Return: ����0 -- ��ȡ�����ݳ��� 
Others: 
*************************************************/
int PcscRecvData( unsigned char * szRecvBuf, int nMaxLen)
{ 
    int nLen = 0;
    //�����ݽ����ж�
    nLen = nMaxLen > s_RecvLen ? s_RecvLen: nMaxLen ;
    s_RecvLen = 0;//����Ϊ��
    return GetCardReaderIns( s_RecvBuf, nLen, szRecvBuf );   
    
}
