#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GlobalDef.h"
#include "CardReaderLayer.h"
#include "TransMssion.h"
#include "CKB51XX.h"
#include "StrOp.h"
#include "TlvOp.h"
#include "Debug.h"
#include "M1Card.h"


S32 ICC_NonContactM1CardActive( U8* szOutAtr, S32* pnAtrLen )
{  
    U8  ucSendData[ MAX_LEN ];
    S32 nSendLen = 0;
    U8  ucRecvData[ MAX_LEN ];
    S32 nRecvLen = 0;

    memset( ucRecvData, 0x0, sizeof(ucRecvData) );
    memset( ucSendData, 0x0, sizeof(ucSendData) );

    ucSendData[0] = 0x32;
    ucSendData[1] = 0x41;
    ucSendData[2] = 0;
    ucSendData[3] = 0;
    nSendLen = 4;
    
    //发送指令
    if( RET_SUCCESS != ICC_WriteDataToCardReader( ucSendData, nSendLen ) )
    {
        return RET_ERROR;
    }
    
    nRecvLen = ICC_ReadDataFromCardReader( ucRecvData, sizeof(ucRecvData) );
    if(  nRecvLen < 0 )
    {
        return RET_ERROR;
    }


    if ( ( NULL != szOutAtr ) && ( NULL != pnAtrLen ) )
    {
        *pnAtrLen = nRecvLen;
        memcpy( szOutAtr, ucRecvData, nRecvLen );

    }
    return  RET_SUCCESS;
}



S32 ICC_NonContactM1CardVerifykey( S32 nSecNo,S32 nKeyMode, U8 *szKey, S32 nKeyLen )
{  
    U8 ucSendData[ MAX_LEN ];
    S32 nSendLen = 0;
    U8 ucRecvData[ MAX_LEN ];
    S32 nRecvLen = 0;

    memset( ucRecvData, 0x0, sizeof(ucRecvData) );
    memset( ucSendData, 0x0, sizeof(ucSendData) );

    ucSendData[0] = 0x32;
    ucSendData[1] = 0x42;
    ucSendData[2] = nSecNo;//扇区号
    ucSendData[3] = nKeyMode;//密钥模式
    nSendLen = 4;
    memcpy( ucSendData+nSendLen, szKey, nKeyLen );
    nSendLen += nKeyLen;
    
    //发送指令
    if( RET_SUCCESS != ICC_WriteDataToCardReader( ucSendData, nSendLen ) )
    {
        return RET_ERROR;
    }

    nRecvLen = ICC_ReadDataFromCardReader( ucRecvData, sizeof(ucRecvData) );
    if( nRecvLen < 0 )
    {
        return RET_ERROR;
    }
    
    return  RET_SUCCESS;
}


S32 ICC_NonContactM1CardRead( S32 nBlock, U8 *szData )
{  
    U8 ucSendData[ MAX_LEN ];
    S32 nSendLen = 0;
    U8 ucRecvData[ MAX_LEN ];
    S32 nRecvLen = 0;

    memset( ucRecvData, 0x0, sizeof(ucRecvData) );
    memset( ucSendData, 0x0, sizeof(ucSendData) );

    ucSendData[0] = 0x32;
    ucSendData[1] = 0x43;
    ucSendData[2] = nBlock;//扇区号
    nSendLen = 3;
  
    //发送指令
    if( RET_SUCCESS != ICC_WriteDataToCardReader( ucSendData, nSendLen ) )
    {
        return RET_ERROR;
    }

    nRecvLen = ICC_ReadDataFromCardReader( ucRecvData, sizeof(ucRecvData) );
    if(  16 !=  nRecvLen )
    {
        return RET_ERROR;
    }

    memcpy( szData, ucRecvData, nRecvLen );
    return  nRecvLen;//返回长度
}


S32 ICC_NonContactM1CardWrite( S32 nBlock, U8 *szData, S32 nDataLen  )
{  
    U8 ucSendData[ MAX_LEN ];
    S32 nSendLen = 0;
    U8 ucRecvData[ MAX_LEN ];
    S32 nRecvLen = 0;

    memset( ucRecvData, 0x0, sizeof(ucRecvData) );
    memset( ucSendData, 0x0, sizeof(ucSendData) );

    ucSendData[0] = 0x32;
    ucSendData[1] = 0x44;
    ucSendData[2] = nBlock;//扇区号
    nSendLen = 3;
    memcpy( ucSendData+nSendLen, szData, nDataLen );
    nSendLen += nDataLen;    
    
    //发送指令
    if( RET_SUCCESS != ICC_WriteDataToCardReader( ucSendData, nSendLen ) )
    {
        return RET_ERROR;
    }

    nRecvLen = ICC_ReadDataFromCardReader( ucRecvData, sizeof(ucRecvData) );
    if(  nRecvLen < 0 )
    {
        return RET_ERROR;
    }
    
    return  RET_SUCCESS;//返回长度
}



