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
Description: ���ն˴���
Input: nAuxIndex -- ���ں�
         nBaud       -- ������
Output: none
Return: ERR_SUCCESS -- �ɹ���other -- ʧ��
Others: ���������nBaud Ϊ�Ƿ�ֵ����Ĭ��Ϊ9600
*************************************************/
S32 ICC_OpenLinuxAux( const S32 nTermType,const S32 nAuxIndex, const S32 nBaud, const S32 nLockKeyboard );
S32 ICC_OpenLinux( char* szTtyName, const S32 nBaud );

/**************************************************
Function:ICC_CloseLinuxAux
Description: �رմ����
Input: none
Output:none
Return: ERR_SUCCESS  -- �ɹ�
Others: 
*************************************************/
S32 ICC_CloseLinuxAux( const S32 nLockKeyboard );
S32 ICC_CloseLinux( void );

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
S32 ICC_ReadLinuxAux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition );
S32 ICC_ReadLinux( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition );

/**************************************************
Function:ICC_WriteLinuxAux
Description: �򸨿ڷ�������
Input: pInData -- ���͵�����
          nLen   --- ���͵����ݳ���
          
Output:none
Return: >0 -- ���͵����ݳ���  others -- ʧ��
Others: 
*************************************************/
S32 ICC_WriteLinuxAux( U8* szData, S32 nDataLen );
S32 ICC_WriteLinux( U8* pInData, S32 nLen );




void ClearReceiveBuffer( void );


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/






#endif
