#ifndef _STR_OP_H_
#define _STR_OP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "GlobalDef.h"
U8 GetDataLRC( U8 *pData, S32 iLen );
void SplitData( U8 *pInData, S32 iInLen, U8 * pOutData );
void MergeData( U8 *pInData, S32 iInLen, U8 * pOutData );
void GetTransDate( U8 *szBuff );
S32 AsciiToHex( U8 * szAscii, S32 nAsciiLen, U8 * szHex  );
S32 AsciiToCn( U8 * szAscii, S32 nAsciiLen, U8 * szCn , S32 nCnLen );
S32 AsciiToBcd( U8 * szAscii, S32 nAsciiLen, U8 * szBcd , S32 nBcdLen );
S32 GetRandom( S32 nRndLen, U8*szRnd );
void GetTransTime( U8* szBuf );
S32 HexToAscii( U8 *szHex, S32 nHexLen, U8* szAscii );



#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif
