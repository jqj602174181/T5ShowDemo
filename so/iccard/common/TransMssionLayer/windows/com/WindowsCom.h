/*!
 * \file Dcomm.h
 * \brief 串口通信读写模块
 */
#ifndef _WINDOWS_COM_H_
#define _WINDOWS_COM_H_


#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_OpenWindowsCom( const S32 nComIndex, const S32 nBaudrate );
S32 ICC_CloseWindowsCom( void );
S32 ICC_ReadWindowsCom( U8* pOutData, S32 nDataSize, const S32 nTimeOut, EndCondition pfReadEnd  );
S32 ICC_WriteWindowsCom( U8* pIntData, S32 nDataSize  );


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif

