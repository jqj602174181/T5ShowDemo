#include <stdio.h>
#include <string.h>
#include "GlobalDef.h"
#include "TransMssion.h"
#include "CardReaderLayer.h"
#include "Debug.h"

#if defined( WINDOWS_EDITION )
#include <Windows.h>
#endif


#if defined( USE_READER_CKB51XX )
#include "CKB51XX.h"
#endif

struct CardReader_t
{
    S32 nCardReader;
    S32 nTimeOut;    
    S32 nCardSlot;
};

static struct CardReader_t  stCardReader;


static S8* ICC_CardCmdGetApduCardType( S8* label )
{
    static S8 apduLabel[64] = {0};
    
    memset( apduLabel, 0x0, sizeof(apduLabel) );  
    
    if( PICC_CARD_SLOT == ICC_CardReaderGetCardSlot() )
    {
        sprintf( apduLabel, "%s", "非接卡片" );
    }
    else if( IS_ICC_SLOT( ICC_CardReaderGetCardSlot() ) )
    {
        sprintf( apduLabel, "%s%d", "接触卡片",  ICC_CardReaderGetCardSlot() - SMART_CARD );
    }
    else if( IS_PSAM_SLOT( ICC_CardReaderGetCardSlot() ) )
    {
        sprintf( apduLabel, "%s%d", "PSAM卡片",  ICC_CardReaderGetCardSlot() - PSAM_CARD );
    }
    else 
    {
        sprintf( apduLabel, "%s", "未知卡片" );   
    }
    
    //标题
    if( label )
    {
        sprintf( apduLabel+strlen(apduLabel), " %s ", label );
    }
    
    return apduLabel;
      
}

void ICC_InitCardReaderParam( TERMPARAM* pTermParam )
{
   
    memset( pTermParam, 0x0, sizeof(TERMPARAM) );
    pTermParam->nInitFlag       = TERM_INIT_VALUE;
    pTermParam->nTransType      = TYPE_COM;
    pTermParam->nCom            = 1;
    pTermParam->nBaud           = DEFAULT_BAUD;
    pTermParam->nTimeOut        = DEFAULT_TIME_OUT;
    pTermParam->lpEndCondition  = NULL;    
    pTermParam->nCardReader     = READER_CKB51XX;
}


S32 ICC_OpenCardReader(  TERMPARAM*  pTermParam  )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
   
    switch(  pTermParam->nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal = ICC_OpenCKB51xxReader( pTermParam );
            break;    
        }
        #endif
        
    }
    
    if( RET_SUCCESS == nRetVal )
    {  
        stCardReader.nCardReader = pTermParam->nCardReader;
        stCardReader.nTimeOut = pTermParam->nTimeOut;
    }
    
    return nRetVal; 
}

S32 ICC_CloseCardReader( void )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal = ICC_CloseCKB51xxReader( );
            break;    
        }
        #endif
        
    }
    
    return nRetVal;     
    
}

S32 ICC_WriteDataToCardReader(  U8 *szData, S32 nDataLen )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal = ICC_WriteDataToCKB51xxReader( szData, nDataLen );
            break;    
        }
        #endif
        
    }
    
     return nRetVal;     
}

S32 ICC_ReadDataFromCardReader(  U8 *szBuf, S32 nBufMaxLen )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal = ICC_ReadDataFromCKB51xxReader( szBuf, nBufMaxLen );
            break;    
        }
        #endif
        
    }
    
    return nRetVal;     
}

S32 ICC_CardReaderCmd(  U8 *szCmd, S32 nCmdLen )
{
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal = ICC_CKB51xxReaderCmd(  szCmd, nCmdLen );
            break;    
        }
        #endif
        
    }
    
    return nRetVal;     
}

S32 ICC_CardReaderPowerOn(  S32 nCardSlot, U8 *szAtr, S32 *pnAtrLen, S32 nTimeOut  )
{
    
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal =  ICC_CKB51xxPowerOn( nCardSlot, szAtr, pnAtrLen,  ( (stCardReader.nTimeOut  >  nTimeOut) ? nTimeOut : stCardReader.nTimeOut)*1000 );
            break;    
        }
        #endif
        
    }
    
    return nRetVal;  
}

S32 ICC_CardReaderPowerOff(  S32 nCardSlot )
{
    
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal =  ICC_CKB51xxPowerOff(  nCardSlot );
            break;    
        }
        #endif
        
    }
    
    return nRetVal;  
}

S32 ICC_CardReaderApdu(   S32 nCardSlot,  U8*C_Apdu,  S32 nCApduLen, U8*R_Apdu, S32 *pnRApduLen  )
{
    
    S32 nRetVal = RET_DEVICE_UNOPEN;
    
    DbgPrintHex( ICC_CardCmdGetApduCardType("C_APDU"), C_Apdu, nCApduLen );
    
    switch(  stCardReader.nCardReader )
    {
        #if defined( USE_READER_CKB51XX )
        case READER_CKB51XX:
        {
            nRetVal =  ICC_CKB51xxAPDU(  nCardSlot, C_Apdu, nCApduLen, R_Apdu, pnRApduLen );
            break;    
        }
        #endif
        
    }
    
    if( RET_SUCCESS == nRetVal )
    {
        DbgPrintHex( ICC_CardCmdGetApduCardType("R_APDU"), R_Apdu, *pnRApduLen );
    }
    else 
    {
        DbgPrintf("获取R_APDU失败，错误码为:%d[%X]\n", nRetVal, nRetVal );     
    }
    
    return nRetVal;  
}

//R20008 add by sulin更正bug，打开串口时会把记录卡片类型清空
S32  ICC_CardReaderReSetCardSlot( )
{
	stCardReader.nCardSlot = 0;
	return RET_SUCCESS;
}  

S32 ICC_CardReaderSetCardSlot(   S32 nCardSlot  )
{
    stCardReader.nCardSlot = nCardSlot;
    return RET_SUCCESS;  
}

S32 ICC_CardReaderGetCardSlot(  void  )
{
    return stCardReader.nCardSlot;  
}

//R20001 add by sulin 增加设置临时波特率功能
S32  ICC_CardReaderSynBps( TERMPARAM *pTermParam, S32 cardType)
{
     S8 ins[32];
     S32 nLen = 0;
     S8 port[32];
	 S32 nRetVal = RET_DEVICE_UNOPEN;
     #define  SLEEP_TIME 30
     
     memset( port, 0x0, sizeof(port) ); 
     memset( ins, 0x0, sizeof(ins) );
     port[ 0 ] = 0x1b;
     port[ 1] = 0x25;
     port[ 2 ] = 0x49;

    ins[ nLen ++ ] = 0x30;
    ins[ nLen ++ ] = 0x01;

    if( PICC_CARD_SLOT == cardType )
    {
        ins[ nLen ++ ] = 0x01;
    }
    else
    {
        ins[ nLen ++ ] = 0x00;
    }

    switch(  pTermParam->nBaud )
    {
        case  9600:
        {
            ins[ nLen ++ ] = 0x00;
            port[ 3 ] = 0x34;
            break;
        }
        case  19200:
        {
            ins[ nLen ++ ] = 0x01;
            port[ 3 ] = 0x33;
            break;
        }
        case  38400:
        {
            ins[ nLen ++ ] = 0x02;
            port[ 3 ] = 0x32;
            break;
        }

        //115200
        default:
        {
            ins[ nLen ++ ] = 0x04;
            port[ 3 ] = 0x31;
            break;
        }

    }
	switch(  stCardReader.nCardReader )
	{
        #if defined( USE_READER_CKB51XX )
		case READER_CKB51XX:
		{
			nRetVal =  ICC_WriteDataToCKB51xxReader( (U8 *)ins, nLen );
			break;    
		}
        #endif

	}
     //根据实际波特率修改卡片波特率
    if( nRetVal != RET_SUCCESS )
    {
       return 1;
    }
    
    ICC_CardReaderCmd( (U8 *)port, 4 );
     
#if !defined( LINUX_EDITION ) 
	Sleep(SLEEP_TIME);
#endif

     ICC_CloseCardReader( ); 
     
#if !defined( LINUX_EDITION ) 
    Sleep(SLEEP_TIME);
#endif

    //  重新打开新波特率串口
    if( RET_SUCCESS !=  ICC_OpenCardReader( pTermParam ) )
    {
        return RET_ERROR;
    }
    ICC_CardReaderCmd( (U8 *)port, 4 );
    
    return RET_SUCCESS;
}

//R20002 add by sulin 增加恢复默认波特率功能
S32 ICC_CardReaderResetBps( S32 cardType )
{

	S8 ins[32];
	S32 nLen = 0;
	S32 nRetVal = RET_DEVICE_UNOPEN;

	memset( ins, 0x0, sizeof(ins) );


	ins[ nLen ++ ] = 0x30;
	ins[ nLen ++ ] = 0x01;

	if( PICC_CARD_SLOT == cardType )
	{
		ins[ nLen ++ ] = 0x01;
	}
	else
	{
		ins[ nLen ++ ] = 0x00;
	}
	ins[ nLen ++ ] = 0x00;//9600

	switch(  stCardReader.nCardReader )
	{
#if defined( USE_READER_CKB51XX )
		case READER_CKB51XX:
			{
				nRetVal =  ICC_WriteDataToCKB51xxReader( (U8 *)ins, nLen );
				break;    
			}
#endif

	}
	return RET_SUCCESS;
}

S32 ICC_CardReaderGetRecStatus( )
{
	switch(  stCardReader.nCardReader )
	{
#if defined( USE_READER_CKB51XX )
		case READER_CKB51XX:
			{
				return ICC_CKB51xxGetRecStatus(); 
			}
#endif
	}
	return RET_SUCCESS;
}



















