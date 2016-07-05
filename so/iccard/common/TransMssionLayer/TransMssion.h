#ifndef _TRANS_MSSION_H_
#define _TRANS_MSSION_H_

#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_OpenTransPort( TERMPARAM* pTermParam );
S32 ICC_WriteDataToTransPort( S8 *szData, S32 nDataLen );
S32 ICC_ReadDataFromTransPort(  S8 *szBuf, S32 nBufMaxLen );
S32 ICC_CloseTransPort( void );

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif
