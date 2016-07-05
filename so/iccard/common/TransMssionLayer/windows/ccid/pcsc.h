/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:     pcsc.h
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
#ifndef _WIN_PCSC_H_
#define _WIN_PCSC_H_
/**************************************************
Function: PcscOpenPort 
Description: ���ն˴���
Input: pTtyName -- �ն���
         nBaud       -- ������
Output: none
Return: ERR_SUCCESS -- �ɹ���other -- ʧ��
Others: ���������nBaud Ϊ�Ƿ�ֵ����Ĭ��Ϊ9600
*************************************************/
int PcscOpenPort( char * pszCardReader );
/**************************************************
Function:PcscClosePort
Description: �رմ����
Input: none
Output:none
Return: ERR_SUCCESS  -- �ɹ�
Others: 
*************************************************/
int PcscClosePort( void );
/**************************************************
Function: PcscSendData
Description: ��������
Input: data  --  ���͵�����
          len  ���ݳ���
Output:none 
Return: 0 -- �ɹ� 
Others: 
*************************************************/
int PcscSendData( const unsigned char *pSendData, const int nLen );
/**************************************************
Function: PcscRecvData
Description: ���ն˽�������
Input: data  --  ���͵�����
          len  ���ݳ���
Output:none 
Return: 0 -- �ɹ� 
Others: 
*************************************************/
int PcscRecvData(  unsigned char * szRecvBuf, int nMaxLen );
#endif