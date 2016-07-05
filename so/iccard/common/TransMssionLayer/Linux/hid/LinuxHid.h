#ifndef _LINUX_HID_H_
#define  _LINUX_HID_H_
#include "GlobalDef.h"

#ifdef __cplusplus
extern "C" {
#endif

    S32 ICC_OpenLinuxHid( const S32 nVid, const S32 nPid );
    S32 ICC_CloseLinuxHid( void );
    S32 ICC_ReadLinuxHid( U8* szRecvBuf, S32 nMaxLen, const S32 nTimeOut, EndCondition lpEndCondition  );
    S32 ICC_WriteLinuxHid( U8* pIntData, S32 nDataSize  );

#ifdef __cplusplus
}
#endif

#endif