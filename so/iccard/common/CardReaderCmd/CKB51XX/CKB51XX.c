#include "GlobalDef.h"
#include"CardReaderLayer.h"
#include "TransMssion.h"
#include "CKB51XX.h"
#include "StrOp.h"
#include <string.h>

struct CKB51XX_PARAM
{
    S32 nTransType;
	//R20003 add by sulin ���ӻ�ȡ������״̬�빦��
	S32 gStatus;
};

static struct CKB51XX_PARAM stCKB51xx;

#ifndef STX 
#define STX (0x02)
#endif

#ifndef ETX
#define ETX (0x03)
#endif

//R20003 add by sulin ���ӻ�ȡ������״̬�빦��
S32 CKB_GetRecStatus( S32 status )
{
	if( status > 0  )//�Ƕ���������״̬
	{
		return stCKB51xx.gStatus= -( ( status / 256 ) / 16 * 1000 + ( status % 256 ) ) ;
	}
	else
	{
		return stCKB51xx.gStatus = status;
	}
}

//R20003 add by sulin ���ӻ�ȡ������״̬�빦��
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
    
    //Ѱ��ͷ��ʼλ��
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
   
    //���ݰ�ͷ��STX��
    szPacket[ nLen++ ] = STX;

    //���ݵ�Ԫ���ȣ�Data_len��:�贫������ݵ�ԪData ���ֵĳ��ȣ����ֽ���ǰ�����ֽ��ں���16 ���Ʊ�ʾ��  
    ucTemp = (U8)(( nDataLen >>8 )&0xff);
    SplitData( &ucTemp, 1, szPacket + nLen );
    nLen += 2;
    ucTemp = (U8)( nDataLen&0xff );
    SplitData( &ucTemp, 1, szPacket + nLen );
    nLen += 2;
    
    //�贫������ݵ�Ԫ(Data)
    SplitData( szData, nDataLen, szPacket + nLen );
    nLen += 2*nDataLen;
    
    //�����������ֵ��LRC��:Data �������ݸ��ֽ����ֵ
    ucTemp  = GetDataLRC( szData, nDataLen );
    SplitData( &ucTemp, 1, szPacket + nLen );
    nLen += 2;
    
    //���ݰ�β��ETX��
    szPacket[ nLen++ ] = ETX;    
    
    return nLen;
}

static S32 CKB_ParsePacket( U8* szPacket, S32 nPacketLen, U8* szData, S32 *pnDataLen )
{
    U8  ucPacketBody[ PACKET_LEN ];//���ݰ���
    U8  ucBody[ PACKET_LEN ];//���ݰ���
    S32 nBodyLen = 0;
    S32 nStatus = 0;
	
    memset( ucPacketBody, 0x0, sizeof(ucPacketBody) );
    nBodyLen = CKB_GetPacketBody( STX, ETX, szPacket, nPacketLen, ucPacketBody );
    if( (nBodyLen < 2)||( nBodyLen%2) )
    {
        return RET_WRONG_PACKET;
    }

    //���ݺϲ��õ�ss:���ݵ�Ԫ����Data_len(2���ֽ�)+�贫������ݵ�ԪData(������)+�������ֵLRC(1���ֽ�)
    memset( ucBody, 0x0, sizeof(ucBody) );
    MergeData( ucPacketBody, nBodyLen ,ucBody );
    nBodyLen = nBodyLen/2;  //�ϲ�������ݳ���
    
    //�������ֵ��LRC���ж�
    if(  GetDataLRC( ucBody + 2, nBodyLen - 3 ) == ucBody[ nBodyLen - 1 ] )
    {
        //Data��ǰ2���ֽ�Ϊ״̬��,�ж�״̬���͡�
        nStatus = ucBody[2]*256 + ucBody[3];
        
        //Data��ʵ�ʳ���Data_len,Ϊ16���ơ�
        nBodyLen = ucBody[0]*256+ucBody[1];   

        memcpy( szData,  ucBody+4, nBodyLen-2 );
        *pnDataLen = nBodyLen-2;
		
		//R20003 add by sulin ���ӻ�ȡ������״̬�빦��
		CKB_GetRecStatus( nStatus );
		
        return nStatus;//����״̬��
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
		  //R20003 add by sulin ���ӻ�ȡ������״̬�빦��
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
    U8 ucPacket[ PACKET_LEN ] = {0};//���͵����ݰ�
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
   
    //�������ݵ�����
    if( nLen == ICC_WriteDataToTransPort( (S8 *)ucPacket, nLen ) )
    {
        nRetVal = RET_SUCCESS;    
    }
    
    return nRetVal;
}

S32 ICC_ReadDataFromCKB51xxReader(  U8 *szBuf, S32 nBufMaxLen )
{
 
    S32 nReadLen = 0;
    U8 ucPacket[ PACKET_LEN ];//���ݰ�
    U8 ucTemp[ PACKET_LEN ];
    S32 nRetVal = RET_WRONG_PACKET;
   
    memset( ucPacket, 0x0, sizeof(ucPacket) );
    memset( ucTemp, 0x0, sizeof(ucTemp) );

	//R20003 add by sulin ���ӻ�ȡ������״̬�빦��
	CKB_ReSetStatus( );
    
    /*!<������*/
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
        /*!<TODO: PCSC ��δʵ��*/ 
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
    
    //�������ݵ�����
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
     
    //�Ӵ�ʽIC���ϵ�ָ���ϸ˵����IC��ָ�
    if( IS_ICC_SLOT( nCardSlot ) || IS_PSAM_SLOT( nCardSlot ) )
    {
        ucSendData[0] = 0x32;
        ucSendData[1] = 0x22;
        ucSendData[2] = ( nTimeOut >> 8 ) & 0xFF;
        ucSendData[3] = nTimeOut & 0xFF;
        ucSendData[4] = nCardSlot; 
        nSendLen = 5;
    }
    //�ǽӴ�ʽIC���ϵ�ָ���ϸ˵����IC��ָ�
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
     
    //Ӧ�ò������ָ��
    ucSendData[ nSendLen++ ] = 0x32;
    ucSendData[ nSendLen++ ] = 0x26;
    
    //�ǽӴ�ʽIC���µ�ָ��
    if( PICC_CARD_SLOT == nCardSlot )
    {
        ucSendData[ nSendLen++ ] = PICC_CARD_SLOT;
    }
     //�Ӵ�ʽIC���µ�ָ��
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




















