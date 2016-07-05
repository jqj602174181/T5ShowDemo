#include <stdio.h>
#include <string.h>
#include "GlobalDef.h"
#include "CardReaderLayer.h"
#include "TransMssion.h"
#include "CKB51XX.h"
#include "StrOp.h"
#include "ICC_CardCmd.h"
#include "Debug.h"

typedef struct 
{
    U8 CLA;
    U8 INS;
    U8 P1;
    U8 P2;
    U8 Lc;
    U8 Data[ MAX_LEN ];
    U8 HasLe;
    U8 Le;
    U8 RApdu[ MAX_LEN ];
    S32 RApduLen;      
} CardCmd;

static struct 
{
    S32 nErrorCode;    
}
stCardCmdParam = { 0xAAAA };

static struct 
{
    S32  errCode;
    S8 msg[128]; 
} 
s_ErrMsg[]=
{
    {0x9000,"�ɹ�ִ��"},
    {0x6200,"��Ϣδ�ṩ"},
    {0x6281,"������Ϣ���ܳ���"},
    {0x6282,"�ļ�����С��Le"},
    {0x6283,"ѡ�е��ļ���Ч"},
    {0x6284,"FCI��ʽ��P2ָ���Ĳ���"},
    {0x6300,"����ʧ��"},
    {0x63CF,"У��ʧ��"},//F ����߱�ʾX
    {0x6400,"״̬��־λû�б�"},
    {0x6581,"�ڴ�ʧ��"},
    {0x6700,"���ȴ���"},
    {0x6882,"��֧�ְ�ȫ����"},
    {0x6981,"�������ļ��ṹ�����ݣ���ǰ�ļ��������ļ�"},
    {0x6982,"����������AC�������㣬û��У��PIN"},
    {0x6983,"��֤����������PIN������"},
    {0x6984,"�������Ч�����õ�������Ч"},
    {0x6985,"ʹ������������"},
    {0x6986,"����������ִ������������������INS�д�"},
    {0x6987,"MAC��ʧ"},
    {0x6988,"MAC����ȷ"},
    {0x698D,"����"},
    {0x6A80,"�������������ȷ"},
    {0x6A81,"���ܲ�֧�֣�����������Ŀ¼��Ч��Ӧ������"},
    {0x6A82,"���ļ�δ�ҵ�"},
    {0x6A83,"�ü�¼δ�ҵ�"},
    {0x6A84,"�ļ�Ԥ���ռ䲻��"},
    {0x6A86,"P1��P2����ȷ"},
    {0x6A88,"��������δ�ҵ�"},
    {0x6B00,"��������ƫ�Ƶ�ַ������EF��"},
    {0x6CFF,"Le���ȴ���ʵ�ʳ�����"},
    {0x6E00,"��֧�ֵ��ࣺCLA�д�"},
    {0x6F00,"������Ч"},
    {0x6D00,"��֧�ֵ�ָ�����"},
    {0x9301,"�ʽ���"},
    {0x9302,"MAC��Ч"},
    {0x9303,"Ӧ�ñ���������"},
    {0x9401,"�����ʽ���"},
    {0x9402,"���׼������ﵽ���ֵ"},
    {0x9403,"��Կ������֧��"},
    {0x9406,"����MAC������"},
    {0x6900,"���ܴ���"},
    {0x6901,"������ܣ���Ч״̬��"},
    {0x61FF,"�跢GET RESPONSE����"},
    {0x6600,"����ͨѶ��ʱ"},
    {0x6601,"�����ַ���ż��"},
    {0x6602,"У��Ͳ���"},
    {0x6603,"��ǰDF�ļ���FCI"},
    {0x6604,"��ǰDF�ļ���SF��KF"},
    //���״̬Ϊ�Զ����
    {0xAAAA,"��IC��ʧ��"}
};

S32 ICC_CardCmdGetErrCode( void )
{
    return stCardCmdParam.nErrorCode;
}

S8* ICC_CardCmdShowErrMsg( S32 nErrorCode  )
{
    S32 i = 0;
    static S8 local_ErrMsg[ MAX_LEN ];
    memset(  local_ErrMsg, 0x0, sizeof( local_ErrMsg ) );
    
    for( i=0; i<TABLESIZE(s_ErrMsg); i++ )
    {

        if( 0x63CF == (nErrorCode|0xF) )
        {
            sprintf( local_ErrMsg,"ErrCode: 0x%X: %s ����������%d", nErrorCode, s_ErrMsg[i].msg , (nErrorCode&0xF) );
            return local_ErrMsg;
        }
        else if ( 0x6CFF ==  (nErrorCode|0xFF) )
        {
            sprintf( local_ErrMsg,"ErrCode: %X: %s %d", nErrorCode,  s_ErrMsg[i].msg ,  (nErrorCode&0xFF) );
            return local_ErrMsg;
        }
        else if ( 0x61FF ==  (nErrorCode|0xFF) )
        {
            sprintf( local_ErrMsg,"ErrCode: %X: %s ", nErrorCode,  s_ErrMsg[i].msg  );
            return local_ErrMsg;
        }
        else  if( ( nErrorCode == s_ErrMsg[i].errCode ) )
        {
            sprintf( local_ErrMsg,"ErrCode: %X: %s", nErrorCode,  s_ErrMsg[i].msg );
            return local_ErrMsg;
        }
        
    }
    
    sprintf( local_ErrMsg,"ErrCode: %X: %s ", nErrorCode, "δ֪�����룡");
    
    return local_ErrMsg;
}

S8 * ICC_CardCmdShowLastErrMsg( void  )
{
    return ICC_CardCmdShowErrMsg(stCardCmdParam.nErrorCode  );
}

S32 ICC_CardCmdVisitCOS( U8 *C_Apdu,  S32 nCapduLen, U8 *pResp,  S32 *pRespLen )
{
    S32 nRetVal = RET_ERROR;
    S32 nRespLen = 0;
    U8 ucCapdu[ MAX_LEN ];
    U8 ucResp[ MAX_LEN ];
 
    memset( ucCapdu, 0x0, sizeof(ucCapdu) );
    memset( ucResp, 0x0, sizeof(ucResp) );
    nRespLen = sizeof(ucResp);
    
    //Ӧ�ò������
    nRetVal = ICC_CardReaderApdu(  ICC_CardReaderGetCardSlot(),  C_Apdu, nCapduLen, ucResp,  &nRespLen  );
    if( RET_SUCCESS !=  nRetVal  )
    {
       stCardCmdParam.nErrorCode = 0xAAAA;
        return nRetVal;
    }
    else if( 2 == nRespLen  )
    {
        if( ucResp[0] == 0x61 )
        {
            // ��IC������ȡӦ��GET RESPONSE������ͷ
            ucCapdu[0] = 0;
            ucCapdu[1] = 0xC0;
            ucCapdu[2] = 0;
            ucCapdu[3] = 0;
            ucCapdu[4] = ucResp[1];//������SW2��ֵ
            memset( ucResp, 0x0, sizeof(ucResp) );
            nRespLen = sizeof(ucResp);
            
            nRetVal = ICC_CardReaderApdu( ICC_CardReaderGetCardSlot(),  ucCapdu, 5, ucResp,  &nRespLen );
        }
        else if( ucResp[0] == 0x62 )
        {
            // ����״̬�����뷢�� Le='00'��ȡӦ��GET RESPONSE������
            ucCapdu[0] = 0;
            ucCapdu[1] = 0xC0;
            ucCapdu[2] = 0;
            ucCapdu[3] = 0;
            ucCapdu[4] = 0;
           memset( ucResp, 0x0, sizeof(ucResp) );
            nRespLen = sizeof(ucResp);
            
            nRetVal = ICC_CardReaderApdu( ICC_CardReaderGetCardSlot(),  ucCapdu, 5, ucResp,  &nRespLen );
        }
        else if( ucResp[0] == 0x6C )
        {
             // ��IC�������ط�����ͷ
            memcpy( ucCapdu, C_Apdu,  4 );
            ucCapdu[ 4 ] = ucResp[ 1 ];
            memset( ucResp, 0x0, sizeof(ucResp) );
            nRespLen = sizeof(ucResp);
            
            nRetVal = ICC_CardReaderApdu( ICC_CardReaderGetCardSlot(),  ucCapdu, 5, ucResp,  &nRespLen );
        } 
        
        if( RET_SUCCESS != nRetVal )
        {
            return nRetVal;
        }
              
    }
    
    *pRespLen = nRespLen;
    memcpy( pResp, ucResp, nRespLen );
    stCardCmdParam.nErrorCode = ucResp[ nRespLen - 2 ]*256+ucResp[ nRespLen - 1 ];

    
    return nRetVal;
}


S32 ICC_CardCmd( CardCmd *cmd )
{
    U8  C_APDU[ MAX_LEN ] = {0};
    S32 nCApduLen = 0;
    
    C_APDU[ nCApduLen++ ] = cmd->CLA;
    C_APDU[ nCApduLen++ ] = cmd->INS;
    C_APDU[ nCApduLen++ ] = cmd->P1;
    C_APDU[ nCApduLen++ ] = cmd->P2;            
    
    if( cmd->Lc )
    {
        C_APDU[ nCApduLen++ ] = cmd->Lc;        
        memcpy( C_APDU+nCApduLen,  cmd->Data,  cmd->Lc );
        nCApduLen += cmd->Lc;
    }
    
    if( ICC_CMD_HAS_LE == cmd->HasLe )
    {
        C_APDU[ nCApduLen++ ] = cmd->Le;  
    }
    
    return ICC_CardCmdVisitCOS( C_APDU, nCApduLen, cmd->RApdu,  &(cmd->RApduLen)  );

}

S32 ICC_CardCmdSelectFile( U8 P1, U8 P2, U8 *IccFileName, S8 IccFileLen, U8 *szOutData,  S32 *pnOutDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = 0x00;
    cmd.INS = 0xA4;
    cmd.P1  = P1;
    cmd.P2  = P2;  
    cmd.Lc  = IccFileLen;
    memcpy( cmd.Data,  IccFileName, cmd.Lc );
    cmd.HasLe = ICC_CMD_HAS_LE;
    cmd.Le = 0;  

     nRetVal = ICC_CardCmd( &cmd );
     if( RET_SUCCESS  == nRetVal )
     {
            if( 0x9000 == ICC_CardCmdGetErrCode() || 0x6283 == ICC_CardCmdGetErrCode() )
            {
                if(  szOutData && pnOutDataLen )
                {
                    memcpy( szOutData, cmd.RApdu, cmd.RApduLen-2 );
                    (*pnOutDataLen)= cmd.RApduLen - 2;
                }
            }
            
            if( 0x9000 != ICC_CardCmdGetErrCode() )
            {
                nRetVal = RET_ERROR;    
            }

     }

    return nRetVal;
  
}

S32 ICC_CardCmdReadRecord( U8 P1, U8 P2, U8 Le, U8 *szOutData,  S32 *pnOutDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = 0x00;
    cmd.INS = 0xB2;
    cmd.P1  = P1;
    cmd.P2  = P2;  
    cmd.HasLe = ICC_CMD_HAS_LE;
    cmd.Le = Le;  

    nRetVal = ICC_CardCmd( &cmd );
    if( RET_SUCCESS  == nRetVal )
    {
        nRetVal = RET_ERROR;    
        if( 0x9000 == ICC_CardCmdGetErrCode()  )
        {
            if(  szOutData && pnOutDataLen )
            {
                memcpy( szOutData, cmd.RApdu, cmd.RApduLen-2 );
                (*pnOutDataLen)= cmd.RApduLen - 2;
            }
            nRetVal = RET_SUCCESS; 
        }
    }

    return nRetVal;
  
}

S32 ICC_CardCmdReadSFI( U8 SFI,  U8 Index,  U8 *szOutData,  S32 *pnOutDataLen )
{
    return  ICC_CardCmdReadRecord( Index,  (SFI<<3)|0x04,  0,  szOutData,  pnOutDataLen );     
}

S32 ICC_CardCmdGetProcessOptions( U8 *szIccData, S32 nIccDataLen, U8 *szOutData,  S32 *pnOutDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = 0x80;
    cmd.INS = 0xA8;
    cmd.P1  = 0x00;
    cmd.P2  = 0x00;  
    cmd.HasLe = ICC_CMD_HAS_LE;
    cmd.Le = 0x00;  
    cmd.Lc  = nIccDataLen+2;
    cmd.Data[0] = 0x83;
    cmd.Data[1] = nIccDataLen&0xFF;
    memcpy( cmd.Data+2,  szIccData, cmd.Lc );
    
    nRetVal = ICC_CardCmd( &cmd );
    if( RET_SUCCESS  == nRetVal )
    {
         nRetVal = RET_ERROR;    
         if( 0x9000 == ICC_CardCmdGetErrCode()  )
         {
             if(  szOutData && pnOutDataLen )
             {
                 memcpy( szOutData, cmd.RApdu, cmd.RApduLen-2 );
                 (*pnOutDataLen)= cmd.RApduLen - 2;
             }
             nRetVal = RET_SUCCESS; 
         }
    }

    return nRetVal;
  
}

S32 ICC_CardCmdGenerateAC( U8 P1, U8 *szIccData, S32 nIccDataLen, U8 *szOutData,  S32 *pnOutDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = 0x80;
    cmd.INS = 0xAE;
    cmd.P1  = P1;
    cmd.P2  = 0x00;  
    cmd.HasLe = ICC_CMD_HAS_LE;
    cmd.Le  = 0x00;  
    cmd.Lc  = nIccDataLen;
    memcpy( cmd.Data, szIccData, cmd.Lc );
    
    nRetVal = ICC_CardCmd( &cmd );
    if( RET_SUCCESS  == nRetVal )
    {
         nRetVal = RET_ERROR;    
         if( 0x9000 == ICC_CardCmdGetErrCode()  )
         {
             if(  szOutData && pnOutDataLen )
             {
                 memcpy( szOutData, cmd.RApdu, cmd.RApduLen-2 );
                 (*pnOutDataLen)= cmd.RApduLen - 2;
             }
             nRetVal = RET_SUCCESS; 
         }
         else if( 0x6985 == ICC_CardCmdGetErrCode() )
         {
             nRetVal = RET_GET_GAC_TOO_MUCH;   
         }
            
    }

    return nRetVal;
  
}

S32 ICC_CardCmdGetData( U8 P1, U8 P2, U8 *szOutData,  S32 *pnOutDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = 0x80;
    cmd.INS = 0xCA;
    cmd.P1  = P1;
    cmd.P2  = P2;  
    cmd.HasLe = ICC_CMD_HAS_LE;
    cmd.Le  = 0x00;  
    cmd.Lc  = 0;
  
    nRetVal = ICC_CardCmd( &cmd );
    if( RET_SUCCESS  == nRetVal )
    {
         nRetVal = RET_ERROR;    
         if( 0x9000 == ICC_CardCmdGetErrCode()  )
         {
             if(  szOutData && pnOutDataLen )
             {
                 memcpy( szOutData, cmd.RApdu, cmd.RApduLen-2 );
                 (*pnOutDataLen)= cmd.RApduLen - 2;
             }
             nRetVal = RET_SUCCESS; 
         }

    }

    return nRetVal;
  
}

S32 ICC_CardCmdExternalAuthenticate( U8 *szAuthData,  S32 nAuthDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = 0x00;
    cmd.INS = 0x82;
    cmd.P1  = 0x00;
    cmd.P2  = 0x00;  
    cmd.HasLe = !ICC_CMD_HAS_LE;
    cmd.Lc  = nAuthDataLen&0xFF;
    memcpy( cmd.Data, szAuthData, cmd.Lc );
    
    nRetVal = ICC_CardCmd( &cmd );
    if( RET_SUCCESS  == nRetVal )
    {
         nRetVal = RET_ERROR;    
         if( 0x9000 == ICC_CardCmdGetErrCode()  )
         {
             nRetVal = RET_SUCCESS; 
         }

    }

    return nRetVal;
  
}

//R20007 add by sulin ����λ����ж�
S32 ICC_CardCmdCheckCardIsInSlot( S32 nCardType )
{
	U8 ucSendData[ PACKET_LEN ];
	S32 nSendLen = 0;
	U8 ucRecvData[ PACKET_LEN ];
	S32 nRecvLen = 0;
	S32 nStatus = 0;
	S32 i = 0;
	//*****************************************************************************    
	memset( ucRecvData, 0x0, sizeof(ucRecvData) );
	memset( ucSendData, 0x0, sizeof(ucSendData) );
	if(  ( nCardType >= 0 )&&( nCardType <= 0x1F ) )
	{
		ucSendData[0] = 0x32;
		ucSendData[1] = 0x21;
		ucSendData[2] = nCardType&0xFF;
		nSendLen = 3;
		if( RET_SUCCESS == ICC_WriteDataToCardReader( ucSendData, nSendLen )  )
		{
			ICC_ReadDataFromCardReader( ucRecvData, nRecvLen );
		}
		nStatus =  -ICC_CardReaderGetRecStatus( ); 
		if( ( 1003 == nStatus ) || ( 1004 == nStatus ) || ( 2003 == nStatus ) || ( 2004 == nStatus ) )//�Ӵ�ʽ
		{
			return RET_SUCCESS;
		}		  
	}
	else if( 0xFF == nCardType )
	{
		ucSendData[0] = 0x32;
		ucSendData[1] = 0x24;
		ucSendData[2] = 0;
		ucSendData[3] = 0;
		nSendLen = 4;
		if( RET_SUCCESS == ICC_WriteDataToCardReader( ucSendData, nSendLen )  )
		{
			ICC_ReadDataFromCardReader( ucRecvData, nRecvLen );
		}
		nStatus =  -ICC_CardReaderGetRecStatus( ); 
		if( 0 == nStatus  )//�ǽӴ�
		{
			return RET_SUCCESS;
		}
	}
	else if( 0x2F == nCardType )
	{
		//�Զ��жϣ�Ĭ������ ���жϽӴ�ʽ��Ƭ״̬��ָ���ж��豸  
		/*<  ����û�������豸�����Խӿڣ�ֻ�ܲ�ȡ���3��IC���۵ķ�ʽ */
		for ( i = 0;i< 3; i++)
		{
			nCardType = SMART_CARD +i;
			ucSendData[0] = 0x32;
			ucSendData[1] = 0x21;
			ucSendData[2] = nCardType&0xFF;
			nSendLen = 3;
			if( RET_SUCCESS == ICC_WriteDataToCardReader( ucSendData, nSendLen )  )
			{
				ICC_ReadDataFromCardReader( ucRecvData, nRecvLen );
			}
			nStatus =  -ICC_CardReaderGetRecStatus( ); 
			if( ( 1003 == nStatus ) || ( 1004 == nStatus ) || ( 2003 == nStatus ) || ( 2004 == nStatus ) )//�Ӵ�ʽ
			{
				return SMART_CARD +i;
			}		 
		}
		


		//*****************************************************************************    
		memset( ucRecvData, 0x0, sizeof(ucRecvData) );
		memset( ucSendData, 0x0, sizeof(ucSendData) );

		nCardType = PICC_CARD_SLOT;
		ucSendData[0] = 0x32;
		ucSendData[1] = 0x24;
		ucSendData[2] = 0;
		ucSendData[3] = 0;
		nSendLen = 4;
		if( RET_SUCCESS == ICC_WriteDataToCardReader( ucSendData, nSendLen )  )
		{
			ICC_ReadDataFromCardReader( ucRecvData, nRecvLen );
		}
		nStatus =  -ICC_CardReaderGetRecStatus( ); 
		if( 0 == nStatus  )//�ǽӴ�
		{
			return PICC_CARD_SLOT;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin ����Ǯ������
// Description: ��ѯ��GET BALANCE���������ڶ�ȡ���Ӵ��ۻ����Ǯ����ʵ�ֲ�ѯ���ס� 
// 				 ��ȡ���Ӵ����������֤����ʶ���루PIN����
S32 ICC_CardCmdGetBalance( S32 nAppType, U8 *szBalance, S32 *pLen )
{

	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	//0x02����Ǯ�� 0x01 ���Ӵ���
	if( (0x02 != nAppType) && (0x01 != nAppType) )
	{
		return RET_ERROR;
	}

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp));

	//�淶��2����,5.2.5
	cbCapdu[0]=0x80;
	cbCapdu[1]=0x5c;
	cbCapdu[2]=0x00;
	cbCapdu[3]=nAppType&0xFF;
	cbCapdu[4]=0x04;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu, 5, cbResp, &iLen ) )
	{
		DbgPrintHex("CT_PbocGetBalance cbResp", ( U8* )cbResp,iLen );
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			DbgPrintf("get balance ok now\n");
			memcpy( szBalance, cbResp, iLen-2 );
			(*pLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin ����Ǯ������
/*��ʼ��Ȧ�棨INITIALIZE FOR LOAD���������ڳ�ʼ��Ȧ�潻�ס�*/
S32 ICC_CardCmdInitializeForLoad(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;
	//0x02����Ǯ�� 0x01 ���Ӵ���
	if( (0x02 != nAppType) && (0x01 != nAppType) )
	{
		return RET_ERROR;
	}
	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x80;
	cbCapdu[ iLen++ ] = 0x50;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = nAppType&0xFF;
	cbCapdu[ iLen++ ] = 0x0B;
	memcpy(&cbCapdu[ iLen ], pData, 0x0B );
	iLen += 0x0B;
	cbCapdu[ iLen++ ] = 0x10;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu,  iLen, cbResp, &iLen ) )
	{
		DbgPrintHex("Ȧ���ʼ����������Ϊ",cbResp, iLen  );
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
/*Ȧ�棨CREDIT FOR LOAD����������Ȧ�潻��*/
S32 ICC_CardCmdCreditForLoad( U8* pData,U8 *pRespData,S32 *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	int iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x80;
	cbCapdu[ iLen++ ] = 0x52;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x0B;
	memcpy(&cbCapdu[ iLen ], pData, 0x0B );
	iLen += 0x0B;
	cbCapdu[ iLen++ ] = 0x04;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu,  iLen, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
/*Description��ʼ��Ȧ�ᣨINITIALIZE FOR UNLOAD���������ڳ�ʼ��Ȧ�ύ�ס�*/
S32 ICC_CardCmdInitializeForUnLoad(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;
	//0x02����Ǯ�� 0x01 ���Ӵ���
	if( (0x02 != nAppType) && (0x01 != nAppType) )
	{
		return RET_ERROR;
	}

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x80;
	cbCapdu[ iLen++ ] = 0x50;
	cbCapdu[ iLen++ ] = 0x05;
	cbCapdu[ iLen++ ] = nAppType&0xFF;
	cbCapdu[ iLen++ ] = 0x0B;
	memcpy(&cbCapdu[ iLen ], pData, 0x0B );
	iLen += 0x0B;
	cbCapdu[ iLen++ ] = 0x10;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu,  iLen, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
/*Description:Ȧ�ᣨDEBIT FOR UNLOAD����������Ȧ�ύ��*/
S32 ICC_CardCmdDebitForUnLoad( U8* pData,U8 *pRespData,int *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x80;
	cbCapdu[ iLen++ ] = 0x54;
	cbCapdu[ iLen++ ] = 0x03;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x0B;
	memcpy(&cbCapdu[ iLen ], pData, 0x0B );
	iLen += 0x0B;
	cbCapdu[ iLen++ ] = 0x04;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu,  iLen, cbResp, &iLen ) )
	{
		DbgPrintHex("Ȧ�᷵������Ϊ",cbResp, iLen  );
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin ����Ǯ������
/*Description:READ BINARY �������ڶ�ȡ�������ļ������ݣ��򲿷����ݣ�*/
S32 ICC_CardCmdReadBinary(U8 SFI,U8 Addr,U8 * pValue, U8 *pReadLen )
{

	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp));


	cbCapdu[0]=0x00;
	cbCapdu[1]=0xB0;
	cbCapdu[2]=0x80|(SFI&0x1F);
	cbCapdu[3]=(Addr)&0xFF;
	cbCapdu[4]=(*pReadLen)&0xFF;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu, 5, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pValue, cbResp, iLen-2 );

			(*pReadLen)= iLen - 2;
			DbgPrintf("CT_PbocReadBinary (*pReadLen) =%d\n", (*pReadLen) );
			DbgPrintHex( "cbResp ", cbResp, iLen );
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin ����Ǯ������
//��ʼ�����ѣ�INITIALIZE FOR PURCHASE���������ڳ�ʼ�����ѽ��ס�
S32 ICC_CardCmdInitializeForPurchase(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x80;
	cbCapdu[ iLen++ ] = 0x50;
	cbCapdu[ iLen++ ] = 0x01;
	cbCapdu[ iLen++ ] = nAppType&0xFF;
	cbCapdu[ iLen++ ] = 0x0B;
	memcpy(&cbCapdu[ iLen ], pData, 0x0B );
	iLen += 0x0B;
	cbCapdu[ iLen++ ] = 0xF;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu,  iLen, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
//Description:����/ȡ�֣�DEBIT FOR PURCHASE/CASH WITHDRAW��������������/ȡ�ֽ��ס�
S32 ICC_CardCmdDebitForPurchaseOrCashWithdraw( U8* pData,U8 *pRespData,int *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x80;
	cbCapdu[ iLen++ ] = 0x54;
	cbCapdu[ iLen++ ] = 0x01;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x0F;
	memcpy(&cbCapdu[ iLen ], pData, 0x0F );
	iLen += 0x0F;
	cbCapdu[ iLen++ ] = 0x08;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu,  iLen, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
//Description:READ BINARY �������ڶ�ȡ�������ļ������ݣ��򲿷�����
S32 ICC_CardCmdUpdateBinary(U8 SFI,U8 Addr,U8 * pValue, U8 ucReadLen )
{

	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp));


	cbCapdu[0]=0x04;
	cbCapdu[1]=0xD6;
	cbCapdu[2]=0x80|(SFI&0x1F);
	cbCapdu[3]=(Addr)&0xFF;
	cbCapdu[4]=ucReadLen&0xFF;
	memcpy( &cbCapdu[5],pValue, ucReadLen);

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu, 5+ucReadLen, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin ����Ǯ������
//Description:ICC_CardCmdVerify VERIFY��������У�������������еĸ���ʶ�������ȷ�ԡ�
S32 ICC_CardCmdVerify( U8 *szData, S32 nDataLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x20;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = nDataLen&0xFF;
	memcpy(&cbCapdu[ iLen ], szData, nDataLen );

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu, iLen+nDataLen, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
//Description: GET CHALLENGE �������ڴ� IC ���л��һ�� �����������
S32 ICC_CardCmdGetChallenge(U8* szRnd, S32 nRndLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x84;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = nRndLen&0xFF;

	if( RET_SUCCESS  == ICC_CardCmdVisitCOS( cbCapdu, 5, cbResp, &iLen ) )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( szRnd, cbResp, iLen-2 );//���������
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin ����Ǯ������
//Description: ������Ƭ
S32 ICC_CardCmdAppBlock(U8 P2,U8* szMac, S32 nMacLen )
{
	int iRetVal;
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp) );

	cbCapdu[ iLen++ ] = 0x84;
	cbCapdu[ iLen++ ] = 0x1E;
	cbCapdu[ iLen++ ] = 0x00;
	cbCapdu[ iLen++ ] = P2&0x01;
	cbCapdu[ iLen++ ] = nMacLen&0xFF;

	memcpy(cbCapdu+iLen,szMac,nMacLen);
	iRetVal = ICC_CardCmdVisitCOS( cbCapdu, nMacLen+5, cbResp, &iLen );
	if( RET_SUCCESS  == iRetVal )
	{
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

S32 ICC_SendCardCmd( U8 CLA, U8 INS, U8 P1, U8 P2, U8 *CApduData, S8 CApduLen, S8 IfHasLe, S8 Le, U8 *szOutData,  S32 *pnOutDataLen )
{
    CardCmd cmd;
    S32 nRetVal = RET_ERROR;
     
    memset( &cmd, 0x0, sizeof(cmd) );
    cmd.CLA = CLA;
    cmd.INS = INS;
    cmd.P1  = P1;
    cmd.P2  = P2;  
    cmd.Lc  = CApduLen;
    memcpy( cmd.Data,  CApduData, cmd.Lc );
    cmd.HasLe = IfHasLe;		//	hasle == 1 ��ʾ�з���ֵ
    cmd.Le = Le;				//	��ʾle����ֵ�ĳ���

    nRetVal = ICC_CardCmd( &cmd );
    if( RET_SUCCESS  == nRetVal )
    {
		if( 0x9000 == ICC_CardCmdGetErrCode() || 0x6283 == ICC_CardCmdGetErrCode() )
		{
            if(  szOutData && pnOutDataLen )
            {
                 memcpy( szOutData, cmd.RApdu, cmd.RApduLen-2 );
                 (*pnOutDataLen)= cmd.RApduLen - 2;
            }
		}            
        if( 0x9000 != ICC_CardCmdGetErrCode() )
        {
            nRetVal = RET_ERROR;    
        }
	}
    return nRetVal;
}