#include "GlobalDef.h"
#include "TransMssion.h"

#if defined( WINDOWS_EDITION )
#include "TransMssionWindows.h"
#endif

/*!< R30001 add by qxs ����linux����ͨѶ����*/
#if defined( LINUX_EDITION )
#include "TransMssionLinux.h"
#endif


S32 ICC_OpenTransPort( TERMPARAM* pTermParam )
{
    S32 nRetVal = RET_ERROR;
    
    #if defined( LINUX_EDITION )
    nRetVal = ICC_OpenLinuxTransPort( pTermParam ); /*!< R30001 add by qxs ����linux�򿪸��ڹ���*/
    #endif
    
    #if defined( WINDOWS_EDITION )
    nRetVal = ICC_OpenWindowsTransPort( pTermParam );
    #endif
    
    return nRetVal;    
    
}

S32 ICC_WriteDataToTransPort( S8 *szData, S32 nDataLen )
{
    S32 nRetVal = RET_ERROR;
        
    #if defined( LINUX_EDITION )
    nRetVal = ICC_WriteDataToLinuxTransPort( (U8 *)szData, nDataLen );/*!< R30001 add by qxs ����linux�����ڷ����ݹ���*/
    #endif
    
    #if defined( WINDOWS_EDITION )
    nRetVal = ICC_WriteDataToWindowsTransPort( (U8 *)szData, nDataLen );
    #endif
    
    return nRetVal;    
    
}


S32 ICC_ReadDataFromTransPort(  S8 *szBuf, S32 nBufMaxLen )
{
    S32 nRetVal = RET_ERROR;
        
    #if defined( LINUX_EDITION )
    nRetVal = ICC_ReadDataFromLinuxTransPort( szBuf, nBufMaxLen );/*!< R30001 add by qxs ����linux�Ӹ��ڶ����ݹ���*/
    #endif
    
    #if defined( WINDOWS_EDITION )
    nRetVal = ICC_ReadDataFromWindowsTransPort( szBuf, nBufMaxLen );
    #endif
    
    return nRetVal;    
    
}

S32 ICC_CloseTransPort( void )
{
    S32 nRetVal = RET_ERROR;
    
    #if defined( LINUX_EDITION )
    nRetVal = ICC_CloseLinuxTransPort();/*!< R30001 add by qxs ����linux�رո��ڹ���*/
    #endif
    
    #if defined( WINDOWS_EDITION )
    nRetVal = ICC_CloseWindowsTransPort();
    #endif
    
    return nRetVal;    
    
}
