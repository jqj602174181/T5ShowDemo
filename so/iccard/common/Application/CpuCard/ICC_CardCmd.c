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
    {0x9000,"成功执行"},
    {0x6200,"信息未提供"},
    {0x6281,"回送信息可能出错"},
    {0x6282,"文件长度小于Le"},
    {0x6283,"选中的文件无效"},
    {0x6284,"FCI格式与P2指定的不符"},
    {0x6300,"鉴别失败"},
    {0x63CF,"校验失败"},//F 在这边表示X
    {0x6400,"状态标志位没有变"},
    {0x6581,"内存失败"},
    {0x6700,"长度错误"},
    {0x6882,"不支持安全报文"},
    {0x6981,"命令与文件结构不相容，当前文件非所需文件"},
    {0x6982,"操作条件（AC）不满足，没有校验PIN"},
    {0x6983,"认证方法锁定，PIN被锁定"},
    {0x6984,"随机数无效，引用的数据无效"},
    {0x6985,"使用条件不满足"},
    {0x6986,"不满足命令执行条件（不允许的命令，INS有错）"},
    {0x6987,"MAC丢失"},
    {0x6988,"MAC不正确"},
    {0x698D,"保留"},
    {0x6A80,"数据域参数不正确"},
    {0x6A81,"功能不支持；创建不允许；目录无效；应用锁定"},
    {0x6A82,"该文件未找到"},
    {0x6A83,"该记录未找到"},
    {0x6A84,"文件预留空间不足"},
    {0x6A86,"P1或P2不正确"},
    {0x6A88,"引用数据未找到"},
    {0x6B00,"参数错误（偏移地址超出了EF）"},
    {0x6CFF,"Le长度错误，实际长度是"},
    {0x6E00,"不支持的类：CLA有错"},
    {0x6F00,"数据无效"},
    {0x6D00,"不支持的指令代码"},
    {0x9301,"资金不足"},
    {0x9302,"MAC无效"},
    {0x9303,"应用被永久锁定"},
    {0x9401,"交易资金不足"},
    {0x9402,"交易计数器达到最大值"},
    {0x9403,"密钥索引不支持"},
    {0x9406,"所需MAC不可用"},
    {0x6900,"不能处理"},
    {0x6901,"命令不接受（无效状态）"},
    {0x61FF,"需发GET RESPONSE命令"},
    {0x6600,"接收通讯超时"},
    {0x6601,"接收字符奇偶错"},
    {0x6602,"校验和不对"},
    {0x6603,"当前DF文件无FCI"},
    {0x6604,"当前DF文件无SF或KF"},
    //这个状态为自定义的
    {0xAAAA,"读IC卡失败"}
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
            sprintf( local_ErrMsg,"ErrCode: 0x%X: %s 还允许重试%d", nErrorCode, s_ErrMsg[i].msg , (nErrorCode&0xF) );
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
    
    sprintf( local_ErrMsg,"ErrCode: %X: %s ", nErrorCode, "未知错误码！");
    
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
    
    //应用层命令传输
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
            // 向IC卡发送取应答（GET RESPONSE）命令头
            ucCapdu[0] = 0;
            ucCapdu[1] = 0xC0;
            ucCapdu[2] = 0;
            ucCapdu[3] = 0;
            ucCapdu[4] = ucResp[1];//长度是SW2的值
            memset( ucResp, 0x0, sizeof(ucResp) );
            nRespLen = sizeof(ucResp);
            
            nRetVal = ICC_CardReaderApdu( ICC_CardReaderGetCardSlot(),  ucCapdu, 5, ucResp,  &nRespLen );
        }
        else if( ucResp[0] == 0x62 )
        {
            // 警告状态，必须发送 Le='00'的取应答（GET RESPONSE）命令
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
             // 向IC卡立即重发命令头
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

//R20007 add by sulin 卡在位情况判断
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
		if( ( 1003 == nStatus ) || ( 1004 == nStatus ) || ( 2003 == nStatus ) || ( 2004 == nStatus ) )//接触式
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
		if( 0 == nStatus  )//非接触
		{
			return RET_SUCCESS;
		}
	}
	else if( 0x2F == nCardType )
	{
		//自动判断，默认先用 “判断接触式卡片状态”指令判断设备  
		/*<  由于没有设置设备的属性接口，只能采取最多3个IC卡槽的方式 */
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
			if( ( 1003 == nStatus ) || ( 1004 == nStatus ) || ( 2003 == nStatus ) || ( 2004 == nStatus ) )//接触式
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
		if( 0 == nStatus  )//非接触
		{
			return PICC_CARD_SLOT;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin 电子钱包功能
// Description: 查询余额（GET BALANCE）命令用于读取电子存折或电子钱包余额，实现查询余额交易。 
// 				 读取电子存折余额需验证个人识别码（PIN）。
S32 ICC_CardCmdGetBalance( S32 nAppType, U8 *szBalance, S32 *pLen )
{

	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;

	//0x02电子钱包 0x01 电子存折
	if( (0x02 != nAppType) && (0x01 != nAppType) )
	{
		return RET_ERROR;
	}

	memset( cbCapdu, 0x0, sizeof(cbCapdu) );
	memset( cbResp, 0x0, sizeof(cbResp));

	//规范第2部分,5.2.5
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

//R20005 add by sulin 电子钱包功能
/*初始化圈存（INITIALIZE FOR LOAD）命令用于初始化圈存交易。*/
S32 ICC_CardCmdInitializeForLoad(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;
	//0x02电子钱包 0x01 电子存折
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
		DbgPrintHex("圈提初始化返回数据为",cbResp, iLen  );
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin 电子钱包功能
/*圈存（CREDIT FOR LOAD）命令用于圈存交易*/
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

//R20005 add by sulin 电子钱包功能
/*Description初始化圈提（INITIALIZE FOR UNLOAD）命令用于初始化圈提交易。*/
S32 ICC_CardCmdInitializeForUnLoad(  S32 nAppType, U8* pData, U8 *pRespData,S32 *pRespLen )
{
	U8 cbCapdu[ MAX_LEN ];
	U8 cbResp[ MAX_LEN ];
	S32 iLen = 0;
	//0x02电子钱包 0x01 电子存折
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

//R20005 add by sulin 电子钱包功能
/*Description:圈提（DEBIT FOR UNLOAD）命令用于圈提交易*/
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
		DbgPrintHex("圈提返回数据为",cbResp, iLen  );
		if( (0x90 == cbResp[ iLen - 2 ] )&&( 0x00 == cbResp[ iLen - 1 ]  ) )
		{
			memcpy( pRespData, cbResp, iLen - 2 );
			(*pRespLen) =  iLen - 2;
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;
}

//R20005 add by sulin 电子钱包功能
/*Description:READ BINARY 命令用于读取二进制文件的内容（或部分内容）*/
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

//R20005 add by sulin 电子钱包功能
//初始化消费（INITIALIZE FOR PURCHASE）命令用于初始化消费交易。
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

//R20005 add by sulin 电子钱包功能
//Description:消费/取现（DEBIT FOR PURCHASE/CASH WITHDRAW）命令用于消费/取现交易。
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

//R20005 add by sulin 电子钱包功能
//Description:READ BINARY 命令用于读取二进制文件的内容（或部分内容
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

//R20005 add by sulin 电子钱包功能
//Description:ICC_CardCmdVerify VERIFY命令用于校验命令数据域中的个人识别码的正确性。
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

//R20005 add by sulin 电子钱包功能
//Description: GET CHALLENGE 命令用于从 IC 卡中获得一个 随机数。该随
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
			memcpy( szRnd, cbResp, iLen-2 );//拷贝随机数
			return RET_SUCCESS;
		}
	}
	return RET_ERROR;

}

//R20005 add by sulin 电子钱包功能
//Description: 锁定卡片
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
    cmd.HasLe = IfHasLe;		//	hasle == 1 表示有返回值
    cmd.Le = Le;				//	表示le返回值的长度

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