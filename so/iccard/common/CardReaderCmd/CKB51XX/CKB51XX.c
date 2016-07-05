#include "GlobalDef.h"
#include"CardReaderLayer.h"
#include "TransMssion.h"
#include "CKB51XX.h"
#include "StrOp.h"
#include <string.h>

struct CKB51XX_PARAM
{
    S32 nTransType;
	//R20003 add by sulin 增加获取读卡器状态码功能
	S32 gStatus;
};

static struct CKB51XX_PARAM stCKB51xx;

#ifndef STX 
#define STX (0x02)
#endif

#ifndef ETX
#define ETX (0x03)
#endif

//R20003 add by sulin 增加获取读卡器状态码功能
S32 CKB_GetRecStatus( S32 status )
{
	if( status > 0  )//是读卡器返回状态
	{
		return stCKB51xx.gStatus= -( ( status / 256 ) / 16 * 1000 + ( status % 256 ) ) ;
	}
	else
	{
		return stCKB51xx.gStatus = status;
	}
}

//R20003 add by sulin 增加获取读卡器状态码功能
static void CKB_ReSetStatus(  )
{
	stCKB51xx.gStatus = -3;
}

static S32 DefaultEndCondition( U8 *szData, S32 nDataLen )
{
    if( ETX == szData[ nDataLen-1 ] )
    {
        return RET_SUCCESS;    
    }
    
    return RET_ERROR;
    
}

static S32 CKB_GetPacketBody( U8 head, U8 end, U8 *szInData, S32 nInDataLen,U8* szOutData )
{   
    S32 nLen = -1;
    S32 nBeginPos = 0;
    S32 nEndPos = 0;
    
    //寻找头起始位置
    while( nInDataLen > nBeginPos )
    {
        
        if( head == szInData[ nBeginPos++ ] )
        {

            break;
        }   
    }

    if( nBeginPos >= nInDataLen-1 )
    {        
        return -1;   
    } 
    
    nEndPos = nBeginPos;
    while(  nInDataLen >  nEndPos )
    {

        if(  end == szInData[ nEndPos ] )
        {
            break;
        }
        nEndPos ++;
    }
    
    nLen = nEndPos - nBeginPos;
    if( ( nLen > 0 )&&( nEndPos <= (nInDataLen-1) ) )
    {
        memcpy( szOutData, szInData+nBeginPos, nLen );
        return nLen;   
    }

    return -1;

}

static S32 CKB_MakePacket( U8* szData, S32 nDataLen, U8* szPacket )
{
    S32 nLen = 0;
    U8 ucTemp = 0; 
   
    //数据包头（STX）
    szPacket[ nLen++ ] = STX;

    //数据单元长度（Data_len）:需传输的数据单元Data 部分的长度，高字节在前，低字节在后，以16 进制表示。  
    ucTemp = (U8)(( nDataLen >>8 )&0xff);
    SplitData( &ucTemp, 1, szPacket + nLen );
    nLen += 2;
    ucTemp = (U8)( nDataLen&0xff );
    SplitData( &ucTemp, 1, szPacket + nLen );
    nLen += 2;
    
    //需传输的数据单元(Data)
    SplitData( szData, nDataLen, szPacket + nLen );
    nLen += 2*nDataLen;
    
    //计算冗余检验值（LRC）:Data 部分数据各字节异或值
    ucTemp  = GetDataLRC( szData, nDataLen );
    SplitData( &ucTemp, 1, szPacket + nLen );
    nLen += 2;
    
    //数据包尾（ETX）
    szPacket[ nLen++ ] = ETX;    
    
    return nLen;
}

static S32 CKB_ParsePacket( U8* szPacket, S32 nPacketLen, U8* szData, S32 *pnDataLen )
{
    U8  ucPacketBody[ PACKET_LEN ];//数据包体
    U8  ucBody[ PACKET_LEN ];//数据包体
    S32 nBodyLen = 0;
    S32 nStatus = 0;
	
    memset( ucPacketBody, 0x0, sizeof(ucPacketBody) );
    nBodyLen = CKB_GetPacketBody( STX, ETX, szPacket, nPacketLen, ucPacketBody );
    if( (nBodyLen < 2)||( nBodyLen%2) )
    {
        return RET_WRONG_PACKET;
    }

    //数据合并得到ss:数据单元长度Data_len(2个字节)+需传输的数据单元Data(不定长)+冗余检验值LRC(1个字节)
    memset( ucBody, 0x0, sizeof(ucBody) );
    MergeData( ucPacketBody, nBodyLen ,ucBody );
    nBodyLen = nBodyLen/2;  //合并后的数据长度
    
    //冗余检验值（LRC）判断
    if(  GetDataLRC( ucBody + 2, nBodyLen - 3 ) == ucBody[ nBodyLen - 1 ] )
    {
        //Data的前2个字节为状态码,判断状态类型。
        nStatus = ucBody[2]*256 + ucBody[3];
        
        //Data的实际长度Data_len,为16进制。
        nBodyLen = ucBody[0]*256+ucBody[1];   

        memcpy( szData,  ucBody+4, nBodyLen-2 );
        *pnDataLen = nBodyLen-2;
		
		//R20003 add by sulin 增加获取读卡器状态码功能
		CKB_GetRecStatus( nStatus );
		
        return nStatus;//返回状态码
    }    
    
    return RET_WRONG_PACKET;
}

static S32 CKB_SendAndReciveData( U8 *szSend, S32 nSendLen, U8 *szRecive, S32 nMaxReciveBufLen  )
{
    S32 nRetVal = RET_WRITE_ERROR;
    
    nRetVal = ICC_WriteDataToCKB51xxReader( szSend, nSendLen );
    if( RET_SUCCESS ==  nRetVal )
    {
        nRetVal = ICC_ReadDataFromCKB51xxReader(  szRecive, nMaxReciveBufLen );
        if( nRetVal < 0 )
        {
            return RET_READ_ERROR;
        }
    }
    else 
    {
		  //R20003 add by sulin 增加获取读卡器状态码功能
		  CKB_ReSetStatus( );
          nRetVal = RET_WRITE_ERROR;
    }

    return  nRetVal;
}

S32 ICC_OpenCKB51xxReader( TERMPARAM* pTermParam )
{
    S32 nRetVal = RET_ERROR;
    
    if( !pTermParam->lpEndCondition )
    {
        pTermParam->lpEndCondition = DefaultEndCondition;
    }

    nRetVal = ICC_OpenTransPort( pTermParam );
    if( RET_SUCCESS == nRetVal )
    {
        stCKB51xx.nTransType = pTermParam->nTransType;
    }
    
    return nRetVal;
}

S32 ICC_CloseCKB51xxReader( void )
{
    return ICC_CloseTransPort();
}


S32 ICC_WriteDataToCKB51xxReader(  U8 *szData, S32 nDataLen )
{

    S32 nLen = 0;
    U8 ucPacket[ PACKET_LEN ] = {0};//发送的数据包
    S32 nRetVal = RET_ERROR;
    
    memset( ucPacket, 0x0,  sizeof(ucPacket) );
    if( TYPE_PCSC != stCKB51xx.nTransType )
    {
        nLen = CKB_MakePacket( szData, nDataLen, ucPacket );
    }
    else
    {
        memcpy( ucPacket, szData, nDataLen );
        nLen = nDataLen;    
    }
   
    //发送数据到串口
    if( nLen == ICC_WriteDataToTransPort( (S8 *)ucPacket, nLen ) )
    {
        nRetVal = RET_SUCCESS;    
    }
    
    return nRetVal;
}

S32 ICC_ReadDataFromCKB51xxReader(  U8 *szBuf, S32 nBufMaxLen )
{
 
    S32 nReadLen = 0;
    U8 ucPacket[ PACKET_LEN ];//数据包
    U8 ucTemp[ PACKET_LEN ];
    S32 nRetVal = RET_WRONG_PACKET;
   
    memset( ucPacket, 0x0, sizeof(ucPacket) );
    memset( ucTemp, 0x0, sizeof(ucTemp) );

	//R20003 add by sulin 增加获取读卡器状态码功能
	CKB_ReSetStatus( );
    
    /*!<读数据*/
    nReadLen = ICC_ReadDataFromTransPort( (S8 *)ucPacket, sizeof(ucPacket) );
    if( nReadLen <= 0 )
    {
        return RET_READ_ERROR;
    }
    
    
    if( TYPE_PCSC != stCKB51xx.nTransType )
    {
        if( !CKB_ParsePacket( ucPacket, nReadLen, ucTemp, &nReadLen ) )
        {
            nRetVal = nReadLen;    
        }
    }
    else
    {
        /*!<TODO: PCSC 暂未实现*/ 
		if( !CKB_ParsePacket( ucPacket, nReadLen, ucTemp, &nReadLen ) )
		{
			nRetVal = nReadLen;    
		}
    }
    
    if( nRetVal > 0 )
    {
        nRetVal = nRetVal > nBufMaxLen ? nBufMaxLen : nRetVal; 
        memcpy( szBuf, ucTemp, nRetVal );
    }
    
    return nRetVal;

}

S32 ICC_CKB51xxReaderCmd(  U8 *szCmd, S32 nCmdLen )
{
    S32 nRetVal = RET_ERROR;
    
    //发送数据到串口
    if( nCmdLen == ICC_WriteDataToTransPort( (S8 *)szCmd, nCmdLen ) )
    {
        nRetVal = RET_SUCCESS;    
    }
    
    return nRetVal;
}

S32 ICC_CKB51xxPowerOn( S32 nCardSlot, U8 *szAtr, S32 *pnAtrLen, S32 nTimeOut )
{
    S32 nRetVal = RET_CARDSLOT_ERROR;
    U8 ucSendData[ MAX_LEN ] = {0};
    S32 nSendLen = 0;
    U8 ucRecvData[ MAX_LEN ]  = {0};
    S32 nRecvLen = 0;

    memset( ucRecvData, 0x0, sizeof(ucRecvData) );
    memset( ucSendData, 0x0, sizeof(ucSendData) );
     
    //接触式IC卡上电指令，详细说明见IC卡指令集
    if( IS_ICC_SLOT( nCardSlot ) || IS_PSAM_SLOT( nCardSlot ) )
    {
        ucSendData[0] = 0x32;
        ucSendData[1] = 0x22;
        ucSendData[2] = ( nTimeOut >> 8 ) & 0xFF;
        ucSendData[3] = nTimeOut & 0xFF;
        ucSendData[4] = nCardSlot; 
        nSendLen = 5;
    }
    //非接触式IC卡上电指令，详细说明见IC卡指令集
    else if( PICC_CARD_SLOT == nCardSlot )
    {
        ucSendData[0] = 0x32;
        ucSendData[1] = 0x24;
        ucSendData[2] = ( nTimeOut >> 8 ) & 0xFF;
        ucSendData[3] = nTimeOut & 0xFF;
        nSendLen = 4;
    }
    else
    {
        return nRetVal;
    }

    nRecvLen = CKB_SendAndReciveData(  ucSendData, nSendLen, ucRecvData, sizeof(ucRecvData)  );
    if( nRecvLen < 0 )
    {
        return nRecvLen;
    }
    
    //ATR
    if ( szAtr && pnAtrLen )
    {
        *pnAtrLen = nRecvLen;
        memcpy( szAtr, ucRecvData, nRecvLen );
    }
    
    return  RET_SUCCESS;

}

S32 ICC_CKB51xxPowerOff(  S32 nCardSlot  )
{
    S32 nRetVal = RET_CARDSLOT_ERROR;
    U8 ucSendData[ MAX_LEN ] = {0};
    S32 nSendLen = 0;
    U8 ucRecvData[ MAX_LEN ] = {0};
    S32 nRecvLen = 0;

    memset( ucRecvData, 0x0, sizeof(ucRecvData) );
    memset( ucSendData, 0x0, sizeof(ucSendData) );
 
    if( IS_ICC_SLOT( nCardSlot ) || IS_PSAM_SLOT( nCardSlot ) )
    {
        ucSendData[0] = 0x32;
        ucSendData[1] = 0x23;
        ucSendData[2] = nCardSlot; 
        nSendLen = 3;
    }
    else if( PICC_CARD_SLOT == nCardSlot )
    {
        ucSendData[0] = 0x32;
        ucSendData[1] = 0x25;
        ucSendData[2] = 0;
        ucSendData[3] = 0;
        nSendLen = 4;
    }
    else
    {
        return nRetVal;
    }

    nRecvLen = CKB_SendAndReciveData(  ucSendData, nSendLen, ucRecvData, sizeof(ucRecvData)  );
    if( nRecvLen < 0 )
    {
        return nRecvLen;
    }

    return  RET_SUCCESS;
        
    
}

S32 ICC_CKB51xxAPDU(  S32 nCardSlot,  U8*C_Apdu,  S32 nCApduLen, U8*R_Apdu, S32 *pnRApduLen )
{  
    U8 ucSendData[ PACKET_LEN ] = {0};
    S32 nSendLen = 0;
    U8 ucRecvData[ PACKET_LEN ]  = {0};
    S32 nRecvLen = 0;
     
    //应用层命令传输指令
    ucSendData[ nSendLen++ ] = 0x32;
    ucSendData[ nSendLen++ ] = 0x26;
    
    //非接触式IC卡下电指令
    if( PICC_CARD_SLOT == nCardSlot )
    {
        ucSendData[ nSendLen++ ] = PICC_CARD_SLOT;
    }
     //接触式IC卡下电指令
    else  if( IS_ICC_SLOT( nCardSlot ) || IS_PSAM_SLOT( nCardSlot ) )
    {
        ucSendData[ nSendLen++ ] = nCardSlot&0xFF;
    }
    
    //C- APDU
    memcpy( ucSendData+nSendLen, C_Apdu,  nCApduLen );
    nSendLen += nCApduLen;

    nRecvLen = CKB_SendAndReciveData(  ucSendData, nSendLen, ucRecvData, sizeof(ucRecvData)  );
    if( nRecvLen < 0 )
    {
        return nRecvLen;
    }
    
    memcpy( R_Apdu, ucRecvData, nRecvLen );
    *pnRApduLen = nRecvLen;
    
    return  RET_SUCCESS;
}

S32 ICC_CKB51xxGetRecStatus( )
{
	return stCKB51xx.gStatus;
}




















