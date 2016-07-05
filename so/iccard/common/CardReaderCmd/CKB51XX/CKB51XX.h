#ifndef _CKB51XX_H_
#define _CKB51XX_H_

#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_OpenCKB51xxReader( TERMPARAM* pTermParam );
S32 ICC_CloseCKB51xxReader( void );
S32 ICC_WriteDataToCKB51xxReader(  U8 *szData, S32 nDataLen );
S32 ICC_ReadDataFromCKB51xxReader(  U8 *szBuf, S32 nBufMaxLen );
S32 ICC_CKB51xxReaderCmd(  U8 *szCmd, S32 nCmdLen );
S32 ICC_CKB51xxPowerOn( S32 nCardSlot, U8 *szAtr, S32 *pnAtrLen, S32 nTimeOut );
S32 ICC_CKB51xxPowerOff(  S32 nCardSlot  );
S32 ICC_CKB51xxAPDU(  S32 nCardSlot,  U8*C_Apdu,  S32 nCApduLen, U8*R_Apdu, S32 *pnRApduLen );
S32 ICC_CKB51xxGetRecStatus( );



#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif
