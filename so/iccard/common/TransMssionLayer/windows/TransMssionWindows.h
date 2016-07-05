#ifndef _TRANS_MSSION_WINDOWS_H_
#define _TRANS_MSSION_WINDOWS_H_

#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_OpenWindowsTransPort( TERMPARAM* pTermParam );
S32 ICC_WriteDataToWindowsTransPort( U8 *szData, S32 nDataLen );
S32 ICC_ReadDataFromWindowsTransPort(  S8 *szBuf, S32 nBufMaxLen );
S32 ICC_CloseWindowsTransPort( void );

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif



