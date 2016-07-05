#ifndef _WINDOWS_HID_H_
#define  _WINDOWS_HID_H_

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

S32 ICC_OpenWindowsHid( const S32 nVid, const S32 nPid );
S32 ICC_CloseWindowsHid( void );
S32 ICC_ReadWindowsHid( U8* pOutData, S32 nDataSize, const S32 nTimeOut, EndCondition pfReadEnd  );
S32 ICC_WriteWindowsHid( U8* pIntData, S32 nDataSize  );

#ifdef __cplusplus
}
#endif

#endif
