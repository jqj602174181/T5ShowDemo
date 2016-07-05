#ifndef _CARD_READER_LAYER_H_
#define _CARD_READER_LAYER_H_



#ifdef __cplusplus
extern "C" {
#endif

void ICC_InitCardReaderParam( TERMPARAM* pTermParam );
S32 ICC_OpenCardReader( TERMPARAM* pTermParam );
S32 ICC_CloseCardReader( void );
S32 ICC_WriteDataToCardReader(  U8 *szData, S32 nDataLen );
S32 ICC_ReadDataFromCardReader(  U8 *szBuf, S32 nBufMaxLen );
S32 ICC_CardReaderCmd(  U8 *szCmd, S32 nCmdLen );
S32 ICC_CardReaderPowerOn(  S32 nCardSlot, U8 *szAtr, S32 *pnAtrLen, S32 nTimeOut  );
S32 ICC_CardReaderPowerOff(  S32 nCardSlot );
S32 ICC_CardReaderApdu(   S32 nCardSlot,  U8*C_Apdu,  S32 nCApduLen, U8*R_Apdu, S32 *pnRApduLen  );
S32 ICC_CardReaderSetCardSlot(   S32 nCardSlot  );
S32 ICC_CardReaderGetCardSlot(  void  );
S32 ICC_CardReaderSynBps( TERMPARAM *pTermParam, S32 cardType);
S32 ICC_CardReaderResetBps( S32 cardType );
S32 ICC_CardReaderGetRecStatus( );



#ifdef __cplusplus
}
#endif/*!< __cplusplus*/






#endif
