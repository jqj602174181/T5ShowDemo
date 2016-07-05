#include <string.h>
#include "GlobalDef.h"
#include "TransMssion.h"
#include "TransMssionLinux.h"
#include "Debug.h"
#include <time.h>

#if defined(USE_TYPE_COM) || defined(USE_TYPE_COM_WITHOUT_AUX)
#include "LinuxAux.h"
#endif

#if defined(USE_TYPE_HID)
#include "LinuxHid.h"
#endif


#if defined(USE_TYPE_HID)
#include "read_fifo.h"
#endif

struct TransMssionLinux_t
{
  S32 nTimeOut;
  EndCondition lpEndCondition; 
  S32 nTransType;  
  S32 nLockKeyboard;
};

static struct TransMssionLinux_t stBackUpParam;

void SetBackUpParam(struct TransMssionLinux_t *pBackUpParam)
{
    memcpy(&stBackUpParam, pBackUpParam, sizeof(stBackUpParam));
}

S32 ICC_OpenLinuxTransPort( TERMPARAM* pTermParam )
{
    S32 nRetVal = RET_ERROR;
    
    switch( pTermParam->nTransType )
    {
		#if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
			nRetVal = ICC_OpenLinuxAux( pTermParam->nTermType, pTermParam->nAuxNo, pTermParam->nBaud, pTermParam->nLockKeyboard );
			break;   
        }
        #endif

        #if defined(USE_TYPE_COM_WITHOUT_AUX)
        case TYPE_COM:
        {
            nRetVal = ICC_OpenLinux( pTermParam->szTermPath, pTermParam->nBaud );
            break;   
        }
        #endif
        
        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_OpenLinuxHid( pTermParam->nVID, pTermParam->nPID );
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
            break;    
        } 
        #endif  
       
        #if defined(USE_TYPE_BUF)       
        case  TYPE_BUF:
        {
            DbgPrintf("fifo_open " );

            nRetVal = fifo_open() ;
            break;    
        } 
        #endif       
    }
    
    /*!打开设备成功则存储通信参数*/
    if( RET_SUCCESS == nRetVal )
    {
        stBackUpParam.nTimeOut = pTermParam->nTimeOut;
        stBackUpParam.lpEndCondition = pTermParam->lpEndCondition;
        stBackUpParam.nTransType = pTermParam->nTransType;
		stBackUpParam.nLockKeyboard = pTermParam->nLockKeyboard;
    }
    
    return nRetVal;    
    
}

S32 ICC_WriteDataToLinuxTransPort( U8 *szData, S32 nDataLen )
{

    S32 nRetVal = RET_ERROR;
    
    switch( stBackUpParam.nTransType )
    {
		#if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
			nRetVal = ICC_WriteLinuxAux( szData, nDataLen );
			DbgPrintHex("PACKGET WRITE", szData, nDataLen );
			break;   
        }
        #endif

        #if defined(USE_TYPE_COM_WITHOUT_AUX)
        case TYPE_COM:
        {
            nRetVal = ICC_WriteLinux( szData, nDataLen );
            DbgPrintHex("PACKGET WRITE", szData, nDataLen );
            break;   
        }
        #endif   

        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_WriteLinuxHid( szData, nDataLen  );
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
            break;    
        } 
        #endif

        #if defined(USE_TYPE_BUF)       
        case  TYPE_BUF:
        {
			char buf[1024] ;
			HexToAscii(szData, nDataLen, buf) ;

            nRetVal = fifo_write( szData, nDataLen );
            DbgPrintHex("PACKGET WRITE", szData, nDataLen );
            break;    
        } 
        #endif              
    }
    
    return nRetVal;     
    
}


S32 ICC_ReadDataFromLinuxTransPort(  S8 *szBuf, S32 nBufMaxLen )
{

    S32 nRetVal = RET_ERROR;
    time_t stStartTime,stEndTime; 

    time( &stStartTime );  

    switch( stBackUpParam.nTransType )
    {
		#if defined(USE_TYPE_COM)
        case TYPE_COM:
        {

			nRetVal = ICC_ReadLinuxAux( (U8 *)szBuf, nBufMaxLen, stBackUpParam.nTimeOut,  stBackUpParam.lpEndCondition   );
			DbgPrintHex("PACKGET READ", (U8 *)szBuf, nRetVal );
			break;   
        }
        #endif

        #if defined(USE_TYPE_COM_WITHOUT_AUX)
        case TYPE_COM:
        {
            nRetVal = ICC_ReadLinux( (U8 *)szBuf, nBufMaxLen, stBackUpParam.nTimeOut,  stBackUpParam.lpEndCondition   );
            DbgPrintHex("PACKGET READ", (U8 *)szBuf, nRetVal );
            break;   
        }
        #endif 

        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_ReadLinuxHid((U8 *)szBuf, nBufMaxLen,  stBackUpParam.nTimeOut, stBackUpParam.lpEndCondition  );
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
            break;    
        } 
        #endif       

        #if defined(USE_TYPE_BUF)       
        case  TYPE_BUF:
        {
			

            nRetVal = fifo_read( (U8 *)szBuf, nBufMaxLen, stBackUpParam.nTimeOut );
           
			
			char buf[1024] ;
		
			
            DbgPrintHex("PACKGET READ", (U8 *)szBuf, nRetVal );
            break;    
        } 
        #endif          
    }
    
    time( &stEndTime ); 
    stBackUpParam.nTimeOut = stBackUpParam.nTimeOut - ( stEndTime - stStartTime );
     
    return nRetVal;      
    
}

S32 ICC_CloseLinuxTransPort( void )
{
    S32 nRetVal = RET_ERROR;
    
    switch( stBackUpParam.nTransType )
    {
		#if defined(USE_TYPE_COM)
        case TYPE_COM:
        {
			nRetVal = ICC_CloseLinuxAux( stBackUpParam.nLockKeyboard );
			break;   
        }
        #endif


        #if defined(USE_TYPE_COM_WITHOUT_AUX)
        case TYPE_COM:
        {
            nRetVal = ICC_CloseLinux( );
            break;   
        }
        #endif 

        
        #if defined(USE_TYPE_HID)
        case TYPE_HID:
        {
            nRetVal = ICC_CloseLinuxHid();
            break;    
        } 
        #endif
        
        #if defined(USE_TYPE_PCSC)       
        case TYPE_PCSC:
        {
            break;    
        } 
        #endif   

        #if defined(USE_TYPE_BUF)       
        case  TYPE_BUF:
        {
            nRetVal = fifo_close();
            break;    
        } 
        #endif        
    }
    
    return nRetVal;        
}
