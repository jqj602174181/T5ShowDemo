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
*  Description: window pc/sc通信协议标准
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
#ifndef _WIN_PCSC_H_
#define _WIN_PCSC_H_
/**************************************************
Function: PcscOpenPort 
Description: 打开终端串口
Input: pTtyName -- 终端名
         nBaud       -- 波特率
Output: none
Return: ERR_SUCCESS -- 成功，other -- 失败
Others: 如果波特率nBaud 为非法值，则默认为9600
*************************************************/
int PcscOpenPort( char * pszCardReader );
/**************************************************
Function:PcscClosePort
Description: 关闭传输口
Input: none
Output:none
Return: ERR_SUCCESS  -- 成功
Others: 
*************************************************/
int PcscClosePort( void );
/**************************************************
Function: PcscSendData
Description: 发送数据
Input: data  --  发送的数据
          len  数据长度
Output:none 
Return: 0 -- 成功 
Others: 
*************************************************/
int PcscSendData( const unsigned char *pSendData, const int nLen );
/**************************************************
Function: PcscRecvData
Description: 从终端接收数据
Input: data  --  发送的数据
          len  数据长度
Output:none 
Return: 0 -- 成功 
Others: 
*************************************************/
int PcscRecvData(  unsigned char * szRecvBuf, int nMaxLen );
#endif