#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <assert.h>
#include "GlobalDef.h"

#ifdef __cplusplus
extern "C" {
#endif

void ICC_DPrintLogToFile( const S8 *fmt, ... );
void ICC_DPrintDataHexToFile( S8 *titleName, U8 *data, S32 dataLen );

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#if defined(DEBUG_OUTPUT)

#define DbgPrintf                 ICC_DPrintLogToFile
#define DbgPrintHex           ICC_DPrintDataHexToFile

#else

#define DbgPrintf             
#define DbgPrintHex      
   
#endif/*!<end of #if defined(DEBUG_OUTPUT)*/

#endif 


