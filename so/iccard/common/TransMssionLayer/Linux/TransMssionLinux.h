#ifndef _TRANS_MSSION_WINDOWS_H_
#define _TRANS_MSSION_WINDOWS_H_

#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_OpenLinuxTransPort( TERMPARAM* pTermParam );
S32 ICC_WriteDataToLinuxTransPort( U8 *szData, S32 nDataLen );
S32 ICC_ReadDataFromLinuxTransPort(  S8 *szBuf, S32 nBufMaxLen );
S32 ICC_CloseLinuxTransPort( void );

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif



