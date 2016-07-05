/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:   LinuxAux.h
*
*  Author:      laiwenhui
*
*  Date:        2012-06-14 15:04
*
*  Description: 
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
#ifndef _AUXILIARY_PORT_H_
#define _AUXILIARY_PORT_H_

#include "GlobalDef.h"

#ifdef __cplusplus
extern "C" {
#endif



/**************************************************
Function: ICC_OpenLinuxAux 
Description: 打开终端串口
Input: nAuxIndex -- 辅口号
         nBaud       -- 波特率
Output: none
Return: ERR_SUCCESS -- 成功，other -- 失败
Others: 如果波特率nBaud 为非法值，则默认为9600
*************************************************/
S32 ICC_OpenLinuxAux( const S32 nTermType,const S32 nAuxIndex, const S32 nBaud, const S32 nLockKeyboard );
S32 ICC_OpenLinux( char* szTtyName, const S32 nBaud );

/**************************************************
Function:ICC_CloseLinuxAux
Description: 关闭传输口
Input: none
Output:none
Return: ERR_SUCCESS  -- 成功
Others: 
*************************************************/
S32 ICC_CloseLinuxAux( const S32 nLockKeyboard );
S32 ICC_CloseLinux( void );

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
S32 ICC_ReadLinuxAux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition );
S32 ICC_ReadLinux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition );

/**************************************************
Function:ICC_WriteLinuxAux
Description: 向辅口发送数据
Input: pInData -- 发送的数据
          nLen   --- 发送的数据长度
          
Output:none
Return: >0 -- 发送的数据长度  others -- 失败
Others: 
*************************************************/
S32 ICC_WriteLinuxAux( U8* szData, S32 nDataLen );
S32 ICC_WriteLinux( U8* pInData, S32 nLen );




void ClearReceiveBuffer( void );


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/






#endif
