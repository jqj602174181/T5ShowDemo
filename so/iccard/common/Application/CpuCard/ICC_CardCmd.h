#ifndef  _ICC_CARD_CMD_H_
#define _ICC_CARD_CMD_H_
#include "GlobalDef.h"
#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_CardCmdGetErrCode( void );
S8* ICC_CardCmdShowErrMsg( S32 nErrorCode  );
S8* ICC_CardCmdShowLastErrMsg( void );
S32 ICC_CardCmdVisitCOS( U8 *C_Apdu, S32 nCapduLen, U8 *pResp, S32 *pRespLen );
S32 ICC_CardCmdSelectFile( U8 P1, U8 P2, U8 *IccFileName, S8 IccFileLen, U8 *szOutData, S32 *pnOutDataLen );
S32 ICC_CardCmdReadRecord( U8 P1, U8 P2, U8 Le, U8 *szOutData,  S32 *pnOutDataLen );
S32 ICC_CardCmdReadSFI( U8 SFI, U8 Index, U8 *szOutData,  S32 *pnOutDataLen );
S32 ICC_CardCmdGetProcessOptions( U8 *szIccData,   S32 nIccDataLen, U8 *szOutData, S32 *pnOutDataLen );
S32 ICC_CardCmdGenerateAC( U8 P1, U8 *szIccData, S32 nIccDataLen, U8 *szOutData,  S32 *pnOutDataLen );
S32 ICC_CardCmdGetData( U8 P1, U8 P2, U8 *szOutData,  S32 *pnOutDataLen );
S32 ICC_CardCmdExternalAuthenticate( U8 *szAuthData,  S32 nAuthDataLen );
//R20007 add by sulin 卡在位情况判断
S32 ICC_CardCmdCheckCardIsInSlot( S32 nCardType );

//R20005 add by sulin 电子钱包功能
S32 ICC_CardCmdGetBalance( S32 nAppType, U8 *szBalance, S32 *pLen );
S32 ICC_CardCmdInitializeForLoad(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen );
S32 ICC_CardCmdCreditForLoad( U8* pData,U8 *pRespData,S32 *pRespLen );
S32 ICC_CardCmdInitializeForUnLoad(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen );
S32 ICC_CardCmdDebitForUnLoad( U8* pData,U8 *pRespData,int *pRespLen );
S32 ICC_CardCmdReadBinary(U8 SFI,U8 Addr,U8 * pValue, U8 *pReadLen );
S32 ICC_CardCmdInitializeForPurchase(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen );
S32 ICC_CardCmdDebitForPurchaseOrCashWithdraw( U8* pData,U8 *pRespData,int *pRespLen );
S32 ICC_CardCmdUpdateBinary(U8 SFI,U8 Addr,U8 * pValue, U8 ucReadLen );
S32 ICC_CardCmdVerify( U8 *szData, S32 nDataLen );
S32 ICC_CardCmdGetChallenge(U8* szRnd, S32 nRndLen );
S32 ICC_CardCmdAppBlock(U8 P2,U8* szMac, S32 nMacLen );
//add by chenjianmin-- about operate capdu 
S32 ICC_SendCardCmd( U8 CLA, U8 INS, U8 P1, U8 P2, U8 *CApduData, S8 CApduLen, S8 IfHasLe, S8 Le, U8 *szOutData,  S32 *pnOutDataLen );
//2014 add chenjianmin
int CT_GetChallenge(U8* szRnd, int nRndLen );

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif

