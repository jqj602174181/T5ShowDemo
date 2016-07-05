#include <stdio.h>
#include "GlobalDef.h"
#include "TransMssion.h"
#include "TransMssionWindows.h"

#if defined( USB_TYPE_HID )
#include "WindowsHid.h"
#endif

#if defined(USE_TYPE_COM)
#include "WindowsCom.h"
#endif
#if defined(USE_TYPE_PCSC) //R20004 add by sulin 增加PCSC通信功能
#include "pcsc.h" 
#endif

struct TransMssionWindows_t
{
  S32 nTimeOut;
  EndCondition lpEndCondition; 
  S32 nTransType;  
};

static struct TransMssionWindows_t stBackUpParam;

S32 ICC_OpenWindowsTransPort( TERMPARAM* pTermParam )
{
    S32 nRetVal = RET_ERROR;
    
    switch( pTermParam->nTransType )
    {
        #if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
            nRetVal = ICC_OpenWindowsCom( pTermParam->nCom, pTermParam->nBaud );
            break;    
        }
        #endif
        
        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_OpenWindowsHid( pTermParam->nVID, pTermParam->nPID );
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
			//R20004 add by sulin 增加PCSC通信功能
            nRetVal = PcscOpenPort( NULL );   
            break;    
        } 
        #endif       
    }
    
    /*!<打开设备成功则存储通信参数*/
    if( RET_SUCCESS == nRetVal )
    {
        stBackUpParam.nTimeOut = pTermParam->nTimeOut;
        stBackUpParam.lpEndCondition = pTermParam->lpEndCondition;
        stBackUpParam.nTransType = pTermParam->nTransType;
    }
    
    return nRetVal;    
    
}

S32 ICC_WriteDataToWindowsTransPort( U8 *szData, S32 nDataLen )
{

    S32 nRetVal = RET_ERROR;
    
    switch( stBackUpParam.nTransType )
    {
        #if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
            nRetVal = ICC_WriteWindowsCom( szData, nDataLen  );
            break;    
        }
        #endif
        
        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_WriteWindowsHid( szData, nDataLen  );
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
			//R20004 add by sulin 增加PCSC通信功能
			nRetVal = PcscSendData( szData, nDataLen );
            break;    
        } 
        #endif       
    }
    
    return nRetVal;     
    
}


S32 ICC_ReadDataFromWindowsTransPort(  S8 *szBuf, S32 nBufMaxLen )
{

    S32 nRetVal = RET_ERROR;
    
    switch( stBackUpParam.nTransType )
    {
        #if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
            nRetVal = ICC_ReadWindowsCom( szBuf, nBufMaxLen, stBackUpParam.nTimeOut,  stBackUpParam.lpEndCondition  );
            break;    
        }
        #endif
        
        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_ReadWindowsHid( szBuf, nBufMaxLen,  stBackUpParam.nTimeOut, stBackUpParam.lpEndCondition  );
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
			//R20004 add by sulin 增加PCSC通信功能
			nRetVal = PcscRecvData( szBuf, nBufMaxLen);
            break;    
        } 
        #endif       
    }
    
    return nRetVal;      
    
}

S32 ICC_CloseWindowsTransPort( void )
{
    S32 nRetVal = RET_ERROR;
    
    switch( stBackUpParam.nTransType )
    {
        #if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
            nRetVal = ICC_CloseWindowsCom( );
            break;    
        }
        #endif
        
        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_CloseWindowsHid();
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
			//R20004 add by sulin 增加PCSC通信功能
			nRetVal = PcscClosePort();
            break;    
        } 
        #endif       
    }
    
    return nRetVal;        
}
