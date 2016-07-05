#ifndef _M1_CARD_H_
#define _M1_CARD_H_

#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_NonContactM1CardActive( U8* szOutAtr, S32* pnAtrLen );
S32 ICC_NonContactM1CardVerifykey( S32 nSecNo,S32 nKeyMode, U8 *szKey, S32 nKeyLen );
S32 ICC_NonContactM1CardRead( S32 nBlock, U8 *szData );
S32 ICC_NonContactM1CardWrite( S32 nBlock, U8 *szData, S32 nDataLen  );

#ifdef __cplusplus
}
#endif

#endif
