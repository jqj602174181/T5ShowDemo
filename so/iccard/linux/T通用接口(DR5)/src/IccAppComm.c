#include "GlobalDef.h"
#include "ECash.h"
#include "TransTag.h"
#include "Debug.h"
#include "IccAppComm.h"
#include "CardReaderLayer.h"
#include "ICC_CardCmd.h"
#include "StrOp.h"
#include "TlvOp.h"
#include "pboc.h"
#include "PrjDef.h"
#include <time.h> 
#include <string.h> 
#include <sys/timeb.h>
#include <stdio.h>
#include <unistd.h>
#include <debugLog.h>


static int g_useBpBox = 0;
static TERMPARAM g_sParam;

static ICCARDPARAM g_ICCardParam;
static unsigned char g_szICCardData[1024] = {0};//用于保存从IC卡里获取的数据
static int g_nICCardLen = 0;
static int g_SetSlotFlag = 0;



static TAGLIST s_TagList[]=
{
	{ "A", 1, TAG_PAN, 1 },                          //卡号（20位）                (0x41)
	{ "B", 1, TAG_CARD_HOLDER_NAME, 2 },             //持卡人姓名                  (0x42)
	{ "C", 1, "\x9F\x62", 2 },                       //持卡人证件类型              (0x43)
	{ "D", 1, "\x9F\x61", 2 },                       //持卡人证件号                (0x44)
	{ "E", 1, TAG_TRACK2_EQUIVALENT_DATA, 1 },       //二磁等效数据（39位）        (0x45)
	{ "F", 1, "\x9F\x1F", 2 },                       //一磁等效数据                (0x46)
	{ "G", 1, TAG_VLP_AVAILABLE_FUNDS, 2 },          //电子现金余额（12位）        (0x47)
	{ "H", 1, TAG_VLP_FUNDS_LIMIT_VISA, 2 },         //电子现金余额上限（12位）    (0x48)
	{ "I", 1, "\x5F\x24", 2 },                       //应用失效日期                (0x49)
	{ "J", 1, TAG_PAN_SEQNO, 2 },                    //卡序列号(应用主账号序列号)  (0x4A)
};

static HID s_HID[5]  = 
{
	{0xBCD5,0x0403},
	{0xBCD7,0x0403},
	{0xBE11,0x2B46},
	{0xD003,0x18D1}, //T5 pid,vid
	{0xBB01,0x2B46}  //T5 new pid vid

};
static int HID_Count = 5 ;

void SetWaitCardTimeout( int timeout )
{
	g_nWaitCardTimeout = timeout ;
}

void SetTimeout(int timeout) 
{
	g_sParam.nTimeOut = timeout ;
}

static int FindTagFromPdol(unsigned char *pTag, int nTaglen, unsigned char *pPdol, int nPdolLen)
{
	unsigned char *pTmpDol = pPdol;
	unsigned char szTagName[10];
	int nTagNameLen;
	int nTmpPdolLen = nPdolLen;
	int nRetVal = 0;
	int nTagValueLen = 0;

	DbgPrintHex("PDOL", pPdol, nPdolLen);
	while(nTmpPdolLen)
	{
		memset(szTagName, 0, sizeof(szTagName));
		nTagNameLen = 0;
		nRetVal = ICC_GetTagInDOL( pTmpDol,  nTmpPdolLen, szTagName, &nTagNameLen, &nTagValueLen );
		if(!nRetVal)
		{
			return RET_ERROR;
		}

		DbgPrintHex("解析tag:", szTagName, nTagNameLen);

		if(!memcmp(szTagName, pTag, nTaglen))
		{
			return RET_SUCCESS;
		}
		pTmpDol += nRetVal;
		nTmpPdolLen -= nRetVal;
	}

	return RET_ERROR;
}


void CT_App_SetSlot(int nSlot)
{
	g_SetSlotFlag = 1;
	
	ICC_CardReaderSetCardSlot( nSlot );
}


int DealCardSlot( char cCardType, int *pnSlot)
{
	int nSlot = 0;
	int i = 0;
	int nReturn = 0;
	int nAtrLen = 0;
	unsigned char uszAtr[128] = {0x00};

	if('1' == cCardType)
	{
		if ( g_SetSlotFlag == 1)
		{
			nSlot = ICC_CardReaderGetCardSlot();
		}
		else
		{
			nSlot = SMART_CARD;
		}
		if ( nSlot == PICC_CARD_SLOT)
		{
			return RET_OTHER_ERR;
		}
		*pnSlot = nSlot;
		ICC_CardReaderSetCardSlot( nSlot );
	}
	else if('2' == cCardType)
	{
		ICC_CardReaderSetCardSlot( PICC_CARD_SLOT );
		*pnSlot = PICC_CARD_SLOT;
	}
	else
	{
		if ( g_SetSlotFlag == 0)
		{
			nReturn = ICC_CardCmdCheckCardIsInSlot( SMART_CARD );
			if( RET_SUCCESS == nReturn )
			{
				ICC_CardReaderSetCardSlot( SMART_CARD );
				*pnSlot = SMART_CARD;
				return 0;
			}
		}
		else  /*<  自动判断 最多有三个卡槽 */
		{
			for ( i = 0; i< 3; i++)
			{
				nReturn = ICC_CardCmdCheckCardIsInSlot( SMART_CARD +i );
				if( RET_SUCCESS == nReturn )
				{
					ICC_CardReaderSetCardSlot( SMART_CARD +i );
					*pnSlot = SMART_CARD +i;
					return 0;
				}
			}
		}

		/*<  非接触判断 */
		nReturn = ICC_CardReaderPowerOn( PICC_CARD_SLOT, uszAtr, &nAtrLen, 0 );
		if(RET_SUCCESS == nReturn)
		{
			ICC_CardReaderSetCardSlot( PICC_CARD_SLOT );
			*pnSlot = PICC_CARD_SLOT;
		}

	}
	return 0;

}

 int IccClosePort( void )
{
    if( TYPE_COM == g_sParam.nTransType )
    {
        if( 9600 != g_sParam.nBaud  )
        {
            ICC_CardReaderResetBps(0);
        }

        //根据是否使用BP盒发送转口指令
        if( !g_useBpBox )
        {
            ICC_CardReaderCmd( (unsigned char *)ICC_DEFAULT_25_PORT, 3 );
        }
        else
        {
            ICC_CardReaderCmd( (unsigned char *)ICC_DEFAULT_24_PORT, 3 );
        }
    }

    return ICC_CloseCardReader();
}

 int IccOpenPort( char *pszComNo, char *BpNo, int nTermType, int nBaud, int nTimeout )
{
    int nRetVal = 0;
	int i = 0;

    //初始化通讯参数
    ICC_InitCardReaderParam( &g_sParam );
    //设置时延
    SetTimeout( nTimeout ) ;

    //串口操作
    if( (memcmp(pszComNo,"COM",3) == 0) || (memcmp(pszComNo,"AUX",3) == 0) )
    {
        g_sParam.nCom = atoi(pszComNo+3);
        g_sParam.nBaud = 9600;
        g_sParam.nTransType = TYPE_COM;
    }
    //hid操作
 #if defined( USE_TYPE_HID ) 
    else if( memcmp(pszComNo,"1001",4) == 0 )
    {
        g_sParam.nTransType = TYPE_HID;
      //  g_sParam.nPID = HID_PID_BCD5;
      //  g_sParam.nVID = HID_VID;
	
    }
 #endif
#if defined( USE_TYPE_BUF ) 
	else if( memcmp(pszComNo,"BUF",3) == 0 )
	{
    	g_sParam.nTransType = TYPE_BUF ;
    }
 #endif
    else
    {
        return RET_PARAM_ERR;
    }

	//打开读卡器
	if(g_sParam.nTransType == TYPE_HID)
	{
		for(i = 0;i<HID_Count;i++)
		{
			g_sParam.nPID = s_HID[i].pid;
			g_sParam.nVID = s_HID[i].vid;
			nRetVal = ICC_OpenCardReader( &g_sParam );
			if(nRetVal == RET_SUCCESS)
			{
				break ;
				//return RET_SUCCESS ;
			}
			else if(i == (HID_Count -1) )
			{
				if(nRetVal != RET_SUCCESS)
				{
					return RET_OPEN_PORT_FAIL;
				}
			}
			else
			{
				IccClosePort( );
			}
		}
	}
	else 
	{
		nRetVal = ICC_OpenCardReader( &g_sParam );
		if(RET_SUCCESS != nRetVal)
		{
			return RET_OPEN_PORT_FAIL;
		}
	}
	
	if ( nRetVal != RET_SUCCESS)
	{
	    IccClosePort( );
		return RET_ERROR;
	}

    return RET_SUCCESS;
}

int IccPowerOn_timeout( int icFlag, unsigned char *uszAtr, int *pnAtrLen )
{
	int nReturn = 0;
	int nSlot = 0;
	int i = 0;
	int DetectCardStatus = -1 ;
	int iloop = 0 ;

//解决卡机动画后第一次检查卡在位的延迟;
#if defined( WINDOWS_EDITION )
	Sleep( 200 ) ;
#endif

#if defined( LINUX_EDITION )
	usleep( 200*1000 ) ;
#endif

	DbgPrintf("IccPowerOn  icFlag  %2x \n ", icFlag );
	
	//设置卡类型;
	if( 1 == icFlag )
	{
		nSlot = SMART_CARD ;
	}
	else if( 2 == icFlag )
	{
		nSlot = PICC_CARD_SLOT ;
	}
	ICC_CardReaderSetCardSlot( nSlot );

	//接触式卡:循环检测卡在位;
	if( 1 == icFlag )
	{
		while(1)
		{
			DetectCardStatus = IccCheckCardIsInSlot( SMART_CARD );
			if( DetectCardStatus > 0 )
			{
				break ;
			}
#if defined( WINDOWS_EDITION )
			Sleep(1000) ;
#endif

#if defined( LINUX_EDITION )
			sleep(1) ;
#endif
			iloop ++ ;
			if(iloop >= g_nWaitCardTimeout )
			{
				return RET_TIMEOUT_ERR ;
			}
		}
	}
	
	//第一次上电，如果卡在位，则直接上电返回成功;
	nReturn = ICC_CardReaderPowerOn( nSlot, NULL, NULL ,g_nWaitCardTimeout) ;
	if( RET_SUCCESS == nReturn )
	{
		return RET_SUCCESS ;
	}

	//非接触式卡循环上电;
	iloop = 0 ;
	if(2 == icFlag )
	{
		while( nReturn )
		{	
#if defined( WINDOWS_EDITION )
			Sleep(1000) ;
#endif

#if defined( LINUX_EDITION )
			sleep(1) ;
#endif
			nReturn =  ICC_CardReaderPowerOn( nSlot, NULL, NULL ,0);
			if(nReturn == RET_SUCCESS )
			{
				return RET_SUCCESS ;
			}
			iloop ++ ;
			if( iloop >= g_nWaitCardTimeout )
			{
				return RET_TIMEOUT_ERR ;
			}
		}

	}

	return RET_POWER_ON_FAIL ;

}

int IccPowerOn( int icFlag, unsigned char *uszAtr, int *pnAtrLen )
{
	int nReturn = 0;
	int nSlot = 0;
	int i = 0;

	DbgPrintf("IccPowerOn  icFlag  %2x \n ", icFlag );

	nReturn =  DealCardSlot( icFlag, &nSlot );
	if ( nReturn != 0)
	{
		return nReturn;
	}

	nReturn = ICC_CardReaderPowerOn( nSlot, uszAtr, pnAtrLen ,0);

	return nReturn;
} 

 int IccPowerOff( void )
{
    if( TYPE_PCSC == g_sParam.nTransType )
    {
        return RET_SUCCESS;
    }

    return ICC_CardReaderPowerOff( ICC_CardReaderGetCardSlot() );
}

 int IccTransformUsrTagToOurTag( TAGLIST *pTagList, int  nTagListLen, char *szUsrTag, int nUsrTagLen, char *szOurTag, int *pOurTagLen )
{
    int i = 0;

    for( i = 0; i < nTagListLen; i++ )
    {
        if( ( !memcmp( pTagList[ i ].UsrTag, szUsrTag, nUsrTagLen) )&&( pTagList[ i ].UsrTagLen ==  nUsrTagLen ) )
        {
            memcpy( szOurTag, pTagList[ i ].OurTag, pTagList[ i ].OurTagLen );
            *pOurTagLen = pTagList[ i ].OurTagLen;
            return RET_SUCCESS;
        }
    }

    return RET_ERROR;
}

 int IccOrgDetail(char *szTxData, int *pnTxDataLen, unsigned char *uszRec)
{
    int nLoop = 0,nLen = 0;

    nLen = *pnTxDataLen;
    szTxData[0] += 1;
    szTxData[1] = 0;

    //  授权金额P
    szTxData[nLen++] = 0x50;
    sprintf(szTxData+nLen, "%03d", 12);
    nLen += 3;
    for(nLoop=0; nLoop<6; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[6+nLoop]);
    }
    nLen += 12;
    szTxData[1] += 16;

    //  其它金额Q
    szTxData[nLen++] = 0x51;
    sprintf(szTxData+nLen, "%03d", 12);
    nLen += 3;
    for(nLoop=0; nLoop<6; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[12+nLoop]);
    }
    nLen += 12;
    szTxData[1] += 16;

    //  交易货币代码R
    szTxData[nLen++] = 0x52;
    sprintf(szTxData+nLen, "%03d", 4);
    nLen += 3;
    for(nLoop=0; nLoop<2; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[20+nLoop]);
    }
    nLen += 4;
    szTxData[1] += 8;

    // 交易日期S
    szTxData[nLen++] = 0x53;
    sprintf(szTxData+nLen, "%03d", 6);
    nLen += 3;
    for(nLoop=0; nLoop<3; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[nLoop]);
    }
    nLen += 6;
    szTxData[1] += 10;

    //  交易类型T
    szTxData[nLen++] = 0x54;
    sprintf(szTxData+nLen, "%03d", 2);
    nLen += 3;
    for(nLoop=0; nLoop<1; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[42+nLoop]);
    }
    nLen += 2;
    szTxData[1] += 6;

    //  交易时间U
    szTxData[nLen++] = 0x55;
    sprintf(szTxData+nLen, "%03d", 6);
    nLen += 3;
    for(nLoop=0; nLoop<3; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[3+nLoop]);
    }
    nLen += 6;
    szTxData[1] += 10;

    //  终端国家代码V
    szTxData[nLen++] = 0x56;
    sprintf(szTxData+nLen, "%03d", 4);
    nLen += 3;
    for(nLoop=0; nLoop<2; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[18+nLoop]);
    }
    nLen += 4;
    szTxData[1] += 8;

    //  商户名称W
    szTxData[nLen++] = 0x57;
    sprintf(szTxData+nLen, "%03d", 20);
    nLen += 3;
    memcpy(szTxData+nLen, uszRec+22, 20);
    if(strlen(szTxData + nLen) < 20)
    {
        memset(szTxData+nLen+strlen(szTxData + nLen), 0x20, 20-strlen(szTxData+nLen));
    }
    nLen += 20;
    szTxData[1] += 24;

    //  应用交易计数器（ATC）X
    szTxData[nLen++] = 0x58;
    sprintf(szTxData+nLen, "%03d", 4);
    nLen += 3;
    for(nLoop=0; nLoop<2; nLoop++)
    {
        sprintf(szTxData+nLen+2*nLoop, "%02X", uszRec[43+nLoop]);
    }
    nLen += 4;
    szTxData[1] += 8;
    *pnTxDataLen = nLen;

    return RET_SUCCESS;
}

//从卡片获取第一个卡片应用
 int IccGetAid( unsigned char *szAID, int *pnAIDlen )
{
	int nRetVal = RET_PARAM_ERR;
	AID_LIST *pAidList = NULL;
	int nAidNum = 0;
	int APPpriority = 0;

    #if !defined(SPEED_UP_NO_GET_AID_FROM_CARD)
 	if( ICC_GetAppList(  &pAidList,  &nAidNum  ) )
 	{
 		ICC_FreeAidList( pAidList );
        AsciiToHex( (unsigned char *)"A000000333", 10, szAID );
 	    *pnAIDlen = 5;
 	}
 	else
 	{

        *pnAIDlen = pAidList->AidLen;
        memset( szAID, 0x00, sizeof(szAID));
        memcpy( szAID, pAidList->AID, *pnAIDlen );

 	}
 	#else

    AsciiToHex( (unsigned char *)"A000000333", 10, szAID );
 	*pnAIDlen = 5;

 	#endif

	return RET_SUCCESS;
}

 int IccAppSelect( unsigned char* szAid ,int nAidLen, unsigned char *szFci, int *pnFciLen )
{
	//  应用被选择并确认交易：根据选择命令（SELECT）返回PDOL的TL信息并确认应用是否被锁定
	if( RET_SUCCESS !=  ICC_AppSelect( szAid, nAidLen, szFci, pnFciLen ) )
	{
		if( 0x6283 == ICC_CardCmdGetErrCode() )
		{
			return RET_CARD_LOCK ;
		}
		else if ( 0x9303 == ICC_CardCmdGetErrCode())
		{
			return RET_CARD_LOCK_FOREVER ;
		}
		else
		{
			return RET_SELECT_APP_FAIL;
		}
	}

	return RET_SUCCESS;
}



 int IccTransGenF55( char *szBankName, unsigned char *szICCardData, int nICCardDataLen, char *szTransData, int *pnF55Len, char *szF55 )
{

	unsigned char szTermData[ 2048 ] = {0};
	int nTermDatalen = 0;
	unsigned char szCDOL1[256] = {0};
	int nCDOL1Len = 0;
	unsigned char szCDOL1Data[1024] = {0};
	int nCDOL1DataLen = 0;
	unsigned char szARQC[256] = {0};
	int nARQClen = 0;
	unsigned char szTemp[1024] = {0};
	int nTempLen = 0;

	//组合终端数据
	ICC_GetCDOLTermData( szBankName, szTransData, szTermData, &nTermDatalen );
	//拷贝AIP
	memcpy( szTermData+nTermDatalen, szICCardData, 4 );
	nTermDatalen += 4;

	// 获取生成密文的CDOL1格式
	ICC_GetCDOL1( szICCardData, nICCardDataLen, szCDOL1, &nCDOL1Len );
	DbgPrintHex( "CDOL1格式", szCDOL1, nCDOL1Len );

	//生成CDOL1数据
	if( RET_SUCCESS != ICC_GetCDOLIccData( szCDOL1, nCDOL1Len, szBankName, szTransData, NULL, (char*)szCDOL1Data, &nCDOL1DataLen ) )
	{
		DbgPrintf("组织CDOL1数据失败！ \r\n");
		return RET_ERROR;
	}
	DbgPrintHex( "CDOL1数据", szCDOL1Data, nCDOL1DataLen );

	//第一次GENERATE AC返回ARQC
	if( RET_SUCCESS != ICC_ExecuteGacCmd( CMD_GAC_ARQC, szCDOL1Data, nCDOL1DataLen, szARQC, &nARQClen ) )
	{
		DbgPrintf( "第一次GAC失败，失败原因 %s \r\n", ICC_CardCmdShowLastErrMsg() );
		return RET_ERROR;
	}
	DbgPrintHex( "第一次GAC成功", szARQC, nARQClen );

	//组织F55域
	ICC_GetF55Domain( szARQC, nARQClen, szTermData, nTermDatalen, szTransData, (unsigned char *)szTemp, &nTempLen );
	DbgPrintf( "F55域 %s\n", szTemp );

	*pnF55Len = nTempLen;
	memcpy( szF55, szTemp ,nTempLen );

	return RET_SUCCESS;
}

 int IccTransTC( char *szBankName, unsigned char *szAuthCode, unsigned char *szICCardData, int nICCardDataLen, char *szTransData, int *pnTCLen, char *szTC )
{

	unsigned char szTermData[ 2048 ] = {0};
	int nTermDatalen = 0;
	unsigned char szCDOL2[256] = {0};
	int nCDOL2Len = 0;
	unsigned char szCDOL2Data[1024] = {0};
	int nCDOL2DataLen = 0;
	unsigned char szTmpTC[256] = {0};
	int nTmpTCLen = 0;
	unsigned char szTemp[1024] = {0};
	int nTempLen = 0;

	//组合终端数据
	ICC_GetCDOLTermData( szBankName, szTransData, szTermData, &nTermDatalen );

	//拷贝AIP
	memcpy( szTermData+nTermDatalen, szICCardData, 4 );
	nTermDatalen += 4;

	// 获取生成密文的CDOL2格式
	ICC_GetCDOL2( szICCardData, nICCardDataLen, szCDOL2, &nCDOL2Len );
	DbgPrintHex( "CDOL2格式", szCDOL2, nCDOL2Len );

	//生成CDOL2数据
	if( RET_SUCCESS != ICC_GetCDOLIccData( szCDOL2, nCDOL2Len, szBankName, szTransData, szAuthCode, (char*)szCDOL2Data, &nCDOL2DataLen ) )
	{
		DbgPrintf("组织CDOL2数据失败！ \r\n");
		return RET_ERROR;
	}
	DbgPrintHex( "CDOL2数据", szCDOL2Data, nCDOL2DataLen );

	//第二次GENERATE AC返回TC
	if( RET_SUCCESS != ICC_ExecuteGacCmd( CMD_GAC_TC, szCDOL2Data, nCDOL2DataLen, szTmpTC, &nTmpTCLen ) )
	{
		DbgPrintf( "第二次GAC失败，失败原因 %s \r\n", ICC_CardCmdShowLastErrMsg() );
		return RET_ERROR;
	}
	DbgPrintHex( "第二次GAC成功", szTmpTC, nTmpTCLen );

	//组织F55域
	ICC_GetF55Domain( szTmpTC, nTmpTCLen, szTermData, nTermDatalen, szTransData, (unsigned char *)szTemp, &nTempLen );
	DbgPrintf( "TC——F55域 %s\n", szTemp );

	*pnTCLen = nTempLen;
	memcpy( szTC, szTemp ,nTempLen );

	return RET_SUCCESS;
}

 void IccBCD16ToASC10(unsigned char *szAscStr,  int nAscStrLen, unsigned char *szBcdStr, int nBcdStrLen)
{
	int nLoop = 0, nIndex = 0;
	unsigned long lNum = 0;
	unsigned char szTemp[30] = {0};

	for(nLoop=0; nLoop<nBcdStrLen; nLoop++)
	{
		lNum = (lNum<<8) + (long)szBcdStr[nLoop];
	}
	sprintf(szTemp, "%10u", lNum);
	nIndex = 10 - nAscStrLen;	
	memcpy(szAscStr, szTemp+nIndex, nAscStrLen);
	for(nLoop=0; nLoop<nAscStrLen; nLoop++)
	{
		if(szAscStr[nLoop]==' ')
		{
			szAscStr[nLoop] = '0';
		}
	}
}

 int IccOrgLoadLog( char *szTxData, unsigned char *uszRec )
{

//1	P	圈存前金额	无小数点，单位为分，12位（字节）
//2	Q	圈存后金额	无小数点，单位为分，12位
//3	S	交易日期	YYMMDD，如111202	 6位
//4	U	交易时间	时分秒，如102550	 6位
//5	V	终端国家代码		             4位
//6	W	商户名称						 40位
//7	X	应用交易计数器	按十进制输出	 4位
//                  标签 长度 偏移
    char format[] = { 'P', 6, 2,  \
                      'Q', 6, 8,  \
                      'S', 3, 14, \
                      'U', 3, 17, \
                      'V', 2, 20, \
                      'W', 20,22, \
                      'X', 2, 42  \
                    };

    int i = 0;
    int logLen = 0;
    int loop = ( sizeof( format )/sizeof( format[0] ) );

    for( i=0; i<loop; i+=3 )
    {
        //标签
        szTxData[ logLen++ ] = format[ i ];

        //长度
        sprintf( szTxData+logLen, "%03d", format[i+1]*2 );
        logLen+=3;

        //数据
        if( 'X' == format[i] )
        {
            //应用交易计数器	按十进制输出
            IccBCD16ToASC10( szTxData+logLen, 4, uszRec+format[i+2], format[i+1] );
            logLen += 4;
        }
        else if( 'W' == format[i] )
        {
			memcpy( szTxData+logLen, "                    ", 20 );
            memcpy( szTxData+logLen, uszRec+format[i+2], strlen(uszRec+format[i+2]) );
            logLen += format[i+1];
        }
        else
        {
            HexToAscii( uszRec+format[i+2], format[i+1], szTxData+logLen );
            logLen += format[i+1]*2;
        }
    }

	return logLen;
}

 int IccOrgTransLog( char *szTxData, unsigned char *uszRec )
{
//交易日期                9A      3
//交易时间                9F21    3
//授权金额                9F02    6
//其它金额                9F03    6
//终端国家代码            9F1A    2
//交易货币代码            5F2A    2
//商户名称                9F4E    20
//交易类型                9C      1
//应用交易计数器（ ATC）  9F36    2

//0x50	P	授权金额	    12	  6  
//0x51	Q	其它金额	    12	  12
//0x52	R	交易货币代码    4	  20 
//0x53	S	交易日期	    6	  0
//0x54	T	交易类型	    2	  42
//0x55	U	交易时间	    6	  3 
//0x56	V	终端国家代码    4	  18
//0x57	W	商户名称	    20	  22
//0x58	X	ATC	            4	  43

//                  标签 长度 偏移
    char format[] = { 'P', 6, 6 ,  \
                      'Q', 6, 12,  \
                      'R', 2,  20,  \
                      'S', 3,  0 ,  \
                      'T', 1,  42,  \
                      'U', 3,  3 ,  \
                      'V', 2,  18,  \
                      'W', 20, 22,  \
					  'X', 2,  43   };

    int i = 0;
    int logLen = 0;
    int loop = ( sizeof( format )/sizeof( format[0] ) );

	logLen = strlen(szTxData);
    for( i=0; i<loop; i+=3 )
    {
        //标签
        szTxData[ logLen++ ] = format[ i ];

        //长度和数据
        if( 'X' == format[i] )
        {
            sprintf( szTxData+logLen, "%03d", 4 );
            logLen+=3;
            //应用交易计数器	按十进制输出
            IccBCD16ToASC10( szTxData+logLen, 4, uszRec+format[i+2], format[i+1] );
            logLen += 4;
        }
        else if( 'W' == format[i] )
        {
            sprintf( szTxData+logLen, "%03d", format[i+1] );
            logLen+=3;
			memcpy( szTxData+logLen, "                     ", 20);
            memcpy( szTxData+logLen, uszRec+format[i+2], strlen(uszRec+format[i+2]));
            logLen += 20;
        }
        else
        {
            sprintf( szTxData+logLen, "%03d", format[i+1]*2 );
            logLen+=3;
            HexToAscii( uszRec+format[i+2], format[i+1], szTxData+logLen );
            logLen += format[i+1]*2;
        }
        
    }

	return logLen;
}



/************************************************************************/
/* func:   IccDoSpecialWork                                                                  
/* 数据组织:对 账号、二磁等效数据作特殊处理;注意每一个需求中， 账号、二磁道数据的nTagIndex是不固定的！
/************************************************************************/

S32 IccDoSpecialWork(  S32 nTagIndex, U8* szTagValue, S32 nTagValueLen, U8 *szOutTagValue, S32 *pTagLen )
{
	S32 nRetVal = RET_SUCCESS;
	U8 szTmp[ MAX_LEN ];
	U8 szBuf[ MAX_LEN ];
	S32 i = 0;
	S32 nCount = 0;

	memset( szTmp, 0x0, sizeof(szTmp) );

	switch( nTagIndex )
	{

	case 'A'://01 - 卡号（20位）
	case 'E'://二磁等效数据（39位）
	case 'F'://一磁等效数据（长度可变）;
		{

			HexToAscii( szTagValue, nTagValueLen,  szTmp );

			for( i=1; i<2*nTagValueLen; i++ )
			{
				if( 'F' == szTmp[ i ]  )
				{
					szTmp[ i ] = 0x0;
					break;
				}

			}

			for( i=1; i<2*nTagValueLen; i++ )
			{
				if( 'D' == szTmp[ i ]  )
				{
					szTmp[ i ] = '=';
					break;
				}
			}

			memset( szBuf, 0x0, sizeof(szBuf) );
			sprintf(szBuf, "%c%03d%s", nTagIndex, strlen(szTmp), szTmp );
			*pTagLen = strlen(szBuf);
			memcpy( szOutTagValue, szBuf, *pTagLen );
			break;
		}
	default:
		{
			HexToAscii( szTagValue, nTagValueLen,  szTmp );
			memset( szBuf, 0x0, sizeof(szBuf) );
			sprintf(szBuf, "%c%03d%s", nTagIndex, nTagValueLen*2, szTmp );
			*pTagLen = strlen(szBuf);
			memcpy( szOutTagValue, szBuf, *pTagLen );
			break;
		}

	}

	return nRetVal;
}


int IccHasDF69Tag( char* szTagList )
{
	int i = 0;
	int nDF69Flag = 0;

	while( '\0' != szTagList[i] )
	{
		if( 'Q' == szTagList[i] )
		{
			nDF69Flag = 1;
			break;
		}

		i++;
	}

	return nDF69Flag;
}



S32 IccGetUsrOrderInfo( U8 *szAppData, S32 nAppLen, U8 *szUsrTag, S32 nUsrLen, U8 *szUsrInfo, S32 *pUsrInfoLen  )
{
      S32 nRetVal = RET_ERROR;
      U8 szOurTag[ MAX_LEN ];
      S32 nOurTagLen = 0;
      U8 szTagValue[ MAX_LEN ];
      S32 nTagValueLen = 0;
      U8 *pTmp = NULL;
      U8 szIcStaticData[ MAX_LEN ];
      S32 nStaticLen = 0;
      S32 nLen = 0;

      memset( szOurTag, 0x0, sizeof(szOurTag) );
      //转化用户tag为PBOC标准的tag
      if( RET_SUCCESS == IccTransformUsrTagToOurTag( s_TagList, TABLESIZE(s_TagList), szUsrTag, nUsrLen, szOurTag, &nOurTagLen ) )
      {
          memset( szTagValue, 0x0, sizeof(szTagValue) );
          pTmp = szAppData;
          nLen = nAppLen;

          if( !memcmp(szOurTag,"\x9F\x4F",nOurTagLen ) ||//日志格式
              !memcmp(szOurTag,"\x9F\x13",nOurTagLen ) ||//上次联机应用交易计数器（ATC）寄存器
              !memcmp(szOurTag,"\x9F\x78",nOurTagLen ) ||//电子现金单笔交易限额
              !memcmp(szOurTag,"\x9F\x6D",nOurTagLen ) ||//电子现金重置阈值
              !memcmp(szOurTag,"\x9F\x17",nOurTagLen ) ||//PIN尝试计数器
              !memcmp(szOurTag,"\x9F\x79",nOurTagLen ) ||//电子现金余额
              !memcmp(szOurTag,"\x9F\x77",nOurTagLen ) ||//电子现金余额上限
              !memcmp(szOurTag,"\x9F\x36",nOurTagLen )   //应用交易计数器（ATC）
           )
           {
              memset( szIcStaticData, 0x0, sizeof( szIcStaticData ) );
              if( RET_SUCCESS ==  ICC_CardCmdGetData( szOurTag[0], szOurTag[1], szIcStaticData, &nStaticLen ) )
              {
                   pTmp = szIcStaticData;
                   nLen = nStaticLen;

              }
              else
              {
                 nLen = 0;
              }

           }

           if( RET_SUCCESS == centerm_get_tag_value( szOurTag, nOurTagLen, pTmp, nLen, szTagValue, &nTagValueLen ) )
           {
               if( RET_SUCCESS ==  IccDoSpecialWork( szUsrTag[0], szTagValue, nTagValueLen, szUsrInfo, pUsrInfoLen )  )
               {
                  nRetVal = RET_SUCCESS;
               }
           }
           else
           {
              szUsrInfo[ (*pUsrInfoLen)++ ] = szUsrTag[0];
              szUsrInfo[ (*pUsrInfoLen)++ ] = 0x30;
              szUsrInfo[ (*pUsrInfoLen)++ ] = 0x30;
              szUsrInfo[ (*pUsrInfoLen)++ ] = 0x30;
              nRetVal = RET_SUCCESS;
           }
      }

      return nRetVal;
}



//****************************导出基本函数************************************
int  CT_APP_DetectCard(int nTermType,  char *pszComNo, char *pBpNo, char *picFlag )
{
    int  nReturn = RET_PARAM_ERR;
    int  nBaud = BUAD_RATE;
    char icFlag = 0;
	int nSlot = 0;

    if( !pszComNo || !picFlag )
    {
        return RET_PARAM_ERR;
    }

    icFlag = *picFlag;
    nReturn = IccOpenPort( pszComNo, pBpNo, nTermType, nBaud ,10);
    if( nReturn != RET_SUCCESS)
    {
        return RET_OPEN_PORT_FAIL;
    }

	nReturn = DealCardSlot( icFlag, &nSlot );
	if ( nReturn != 0)
	{
		return nReturn;
	}

    nReturn = ICC_CardCmdCheckCardIsInSlot( nSlot );
    if( RET_SUCCESS == nReturn )
    {
        ICC_CardReaderSetCardSlot( nSlot );
        nReturn = ICC_CardReaderGetRecStatus( );
		if ( nReturn == 0 )
		{
			nReturn = NOTOUCH_CARD_IN_UP;
		}
        else if( nReturn == -1003 )
        {
            nReturn = TOUCH_CARD_IN_UP;//接触在位已经上电
        }
        else if( nReturn == -1004 )
        {
            nReturn = TOUCH_CARD_IN;//接触在位未上电
        }
        else
        {
            nReturn = RET_OTHER_ERR;// 未知错误
        }
    }
    else
    {
        nReturn = ICC_CardReaderGetRecStatus( );
        if( nReturn != -3 )//超时
        {
            nReturn = RET_OTHER_ERR;
        }
    }
   


    IccClosePort();

    return nReturn;
}

int  CT_APP_CardPresent( int nTermType, char *pszComNo, char *pBpNo, char *picFlag )
{
    return CT_APP_DetectCard( nTermType, pszComNo, pBpNo, picFlag );
}

int  CT_APP_PowerOn( char *pszComNo,int nTermType, char *pBpNo, char *picFlag, int *pATRLen, unsigned char *pATR )
{
    int nReturn = RET_PARAM_ERR;
    unsigned char uszAtr[128] = {0};
    int nAtrLen = 0;
    int nBaud = BUAD_RATE;

    if( !pszComNo || !picFlag )
    {
        return RET_PARAM_ERR;
    }
	DbgPrintf("【IC 卡上电】 输入参数 串口 %s IC 卡标志 %s \n", pszComNo,picFlag );
	DbgPrintf("【IC 卡上电】打开串口\n");
    if( IccOpenPort(  pszComNo, pBpNo, nTermType, nBaud, 10 ) != RET_SUCCESS)
    {
		
        return RET_OPEN_PORT_FAIL;
    }
	DbgPrintf("【IC 卡上电】打开串口 SUCC\n");
    if( !IccPowerOn( *picFlag, uszAtr, &nAtrLen ) )
    {
        if( pATRLen && pATR )
        {
            memcpy( pATR, uszAtr, nAtrLen );
			
            *pATRLen = nAtrLen;
        }

        nReturn = RET_SUCCESS;
    }
    else
    {
        nReturn = ICC_CardReaderGetRecStatus( );
    }

    IccClosePort();

    return nReturn;
}

int  CT_APP_PowerOff( char *pszComNo, int nTermType, char *pBpNo, char *picFlag )
{
    int nReturn = RET_PARAM_ERR;
    int nBaud = BUAD_RATE;
	int nSlot = 0;

    if( !pszComNo || !picFlag  )
    {
        return RET_PARAM_ERR;
    }

    if( IccOpenPort(   pszComNo, pBpNo, nTermType, nBaud, 10 ) != RET_SUCCESS )
    {
        return RET_OPEN_PORT_FAIL;
    }


	nReturn = DealCardSlot( *picFlag, &nSlot );
	if ( nReturn != 0)
	{
		return nReturn;
	}

    nReturn = IccPowerOff();
    if( nReturn )
    {
         nReturn = ICC_CardReaderGetRecStatus( );
    }

    IccClosePort();

    return nReturn;
}


int  CT_APP_SendCommand( char *pszComNo, int nTermType, char *BpNo,char *icFlag, int cmdLen, unsigned char *pszCmd, int *pnRespLen, unsigned char *pszResp )
{
    int nReturn = RET_PARAM_ERR;
    int nBaud = BUAD_RATE;
    U8 ucRecvData[ 512 ] = {0};
    int nRecvLen = 0;
    char cicFlag = 0;
	int nSlot = 0;
	char cmd[256+1] = {0};

    if( !pszComNo || !pszCmd || !pszResp || !pnRespLen || !icFlag )
    {
        return RET_PARAM_ERR;
    }
	DbgPrintf("===>test app send\r\n");
	DbgPrintf("==>port:%s term:%d,baud:%d\r\n",pszComNo,nTermType,nBaud);
    nReturn = IccOpenPort( pszComNo, BpNo, nTermType, nBaud, 10 );
	DbgPrintf("==>nReturn:%d\r\n",nReturn);
    if(nReturn != 0)
    {
        IccClosePort();
        return RET_OPEN_PORT_FAIL;
    }

#ifndef AUTH_MODE
    cicFlag = *icFlag;
	nReturn = DealCardSlot( cicFlag, &nSlot );
	if ( nReturn != 0)
	{
		return nReturn;
	}
#endif

	DbgPrintHex("【发送命令】 pszCmd\n", pszCmd, cmdLen);
	if(cmdLen == 4)
	{
		DbgPrintf("cmd len长度为4，补le字节\n");
		memset(cmd,0x00,sizeof(cmd));
		memcpy(cmd,pszCmd,4);
		cmdLen = 5;
		nReturn = ICC_CardCmdVisitCOS( cmd, cmdLen, ucRecvData ,&nRecvLen);
	}
    else
	{
		nReturn = ICC_CardCmdVisitCOS( pszCmd, cmdLen, ucRecvData ,&nRecvLen);
	}
    if(nReturn == RET_SUCCESS)
    {
        memcpy( pszResp, ucRecvData, nRecvLen );
        *pnRespLen = nRecvLen;
        nReturn = RET_SUCCESS;
    }
    else
    {

        nReturn = ICC_CardReaderGetRecStatus( );
        *pnRespLen = 0;
    }

    IccClosePort();

    return nReturn;
}

//****************************导出PBOC函数************************************
int  CT_APP_GetIccInfo( char *pszComNo, int nTermType,  char *pBpNo, int* icFlag, char *pszAidList, char *pszTaglist, int *pnUsrInfoLen, char *pszUserInfo, int nWaitCardTimeout )
{
	int nRetVal = RET_PARAM_ERR;
	unsigned char ucUserTag  = 0;
	unsigned int nLoop   = 0;
	int ntmpLen = 0;

	unsigned char szAID[64] = {0};
	int nAIDlen = 0;
	unsigned char szFci[256] = {0};
	int nFciLen = 0;
	unsigned char szAFL[256] = {0};
	int nAFLlen = 0;
	unsigned char szAIP[256] = {0};
	int nAIPlen = 0;
	unsigned char szTags[256] = {0};
	int nTagsLen = 0;
	unsigned char uszPbocTag[32] = {0};
	int nPbocTagLen = 0;
	unsigned char szICCardData[4096] = {0};//用于保存从IC卡里获取的数据
	int nICCardLen = 0;
	unsigned char szInfo[256] = {0};
	int nInfoLen  = 0;
	unsigned char szPdol[128] = {0};
	int nPdolLen = 0;
	int nSupportGM = 0;
	int DetectCardStatus = -1 ;
	int iloop = 0 ;
	int retvalPowerOn = -1 ;

	if( *icFlag <1 || *icFlag>3 )
	{
		fifo_sendFinish( ) ;
		return RET_PARAM_ERR ;
	}
	/*为了兼容喊话功能，避开卡类型 =3的情况（自动判断）*/
	if(*icFlag == 3)
	{
		*icFlag = 1 ;
	}

	DbgPrintf("【NEW 获取卡片信息】 入参： pszComNo %s ic卡类型%d pszTaglist %s \n", pszComNo, *icFlag, pszTaglist );
	//参数判断
	if( !pszTaglist || !pnUsrInfoLen || !pszUserInfo )
	{
		fifo_sendFinish( ) ;
		DbgPrintf(" 【获取卡片信息】 参数错误！ ") ;
		return nRetVal;
	}
	
	DbgPrintf("【打开串口】\n") ;
	//打开端口
	if( IccOpenPort(  pszComNo, pBpNo, nTermType, BUAD_RATE, nWaitCardTimeout ) != RET_SUCCESS)
	{
		nRetVal = RET_OPEN_PORT_FAIL;
		goto GO_OPEN_PORT_FAIL;
	}

	/*开启T5卡机动画;*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		DbgPrintf("【T5_StartPlayDemo开始】 " );
		nRetVal = T5_StartPlayDemo( *icFlag , STARTPLAYDEMO_TIMEOUT ) ;
		DbgPrintf("【T5_StartPlayDemo结束】retval -- %d ", nRetVal );
		if( RET_SUCCESS != nRetVal )
		{
			goto GO_STARTPLAYDEMO_FAIL ;
		}
	}

	//设置超时时间;
	SetWaitCardTimeout( nWaitCardTimeout ) ;
	DbgPrintf( "超时时间g_ntimeout = %d \n", g_nWaitCardTimeout ) ;

	//上电
	DbgPrintf("【上电】 卡类型%d \n", *icFlag) ;
	nRetVal = IccPowerOn_timeout( *icFlag, NULL, NULL ) ;
	if(nRetVal != RET_SUCCESS )
	{
		DbgPrintf( "上电失败 卡类型 %d \n ", *icFlag ) ;
		//nRetVal = RET_POWER_ON_FAIL ;
		goto GO_POWER_ON_FAIL ;
	}

	//获取应用aid
	if( ( !pszAidList )   || ( strlen(pszAidList) == 0))
	{
		//从卡片获取
		IccGetAid( szAID, &nAIDlen );
		DbgPrintHex( "从卡片获取的aid", szAID, nAIDlen );
	}
	else
	{
		DbgPrintf("【NEW 获取卡片信息】 入参:pszAidList  %s\n ", pszAidList);
		//使用接口传入的参数
		nAIDlen = AsciiToHex( (unsigned char *)pszAidList, strlen(pszAidList), szAID );
		DbgPrintf("使用接口传入的AID %s\n", pszAidList );
	}

	//应用选择
	nRetVal = IccAppSelect( szAID , nAIDlen, szFci, &nFciLen );
	if( RET_SUCCESS != nRetVal && RET_CARD_LOCK != nRetVal )
	{
		DbgPrintHex("应用选择失败", szAID , nAIDlen );
		goto GO_SELECT_APP_FAIL;
	}

	//应用初始化
	nRetVal = ICC_AppInit( BANGK, szFci, nFciLen, szAFL, &nAFLlen, szAIP, &nAIPlen );
	if( RET_SUCCESS != nRetVal )
	{
		DbgPrintHex("应用初始化", szAID , nAIDlen );
		nRetVal = RET_INIT_FAIL;
		goto  GO_INIT_FAIL;
	}

	//保存AIP数据
	memset( szICCardData, 0x00, sizeof( szICCardData) );
	szICCardData[ nICCardLen++ ] = 0x82;
	szICCardData[ nICCardLen++ ] = 0x02;
	szICCardData[ nICCardLen++ ] = szAIP[0];
	szICCardData[ nICCardLen++ ] = szAIP[1];

	//将需要重AFL里读取的数据的tag保存到tag数组里
	memset( szTags, 0x0, sizeof(szTags) );
	for( nLoop=0; nLoop<(int)strlen((pszTaglist)); nLoop++ )
	{
		ucUserTag = pszTaglist[ nLoop ];
		if( RET_SUCCESS == IccTransformUsrTagToOurTag( s_TagList, TABLESIZE(s_TagList), &ucUserTag, 1, (char *)uszPbocTag, &nPbocTagLen ) )
		{
			if( !memcmp(uszPbocTag, "\x9F\x4F", nPbocTagLen ) ||//日志格式
				!memcmp(uszPbocTag, "\x9F\x13", nPbocTagLen ) ||//上次联机应用交易计数器（ATC）寄存器
				!memcmp(uszPbocTag, "\x9F\x78", nPbocTagLen ) ||//电子现金单笔交易限额
				!memcmp(uszPbocTag, "\x9F\x6D", nPbocTagLen ) ||//电子现金重置阈值
				!memcmp(uszPbocTag, "\x9F\x17", nPbocTagLen ) ||//PIN尝试计数器
				!memcmp(uszPbocTag, "\x9F\x79", nPbocTagLen ) ||//电子现金余额
				!memcmp(uszPbocTag, "\x9F\x77", nPbocTagLen ) ||//电子现金余额上限
				!memcmp(uszPbocTag, "\x9F\x36", nPbocTagLen ) || //应用交易计数器（ATC）
				!memcmp(uszPbocTag, "\x82",     nPbocTagLen )   //应用交易计数器（ATC）
				)
			{
				continue;
			}
			else
			{
				memcpy( szTags+nTagsLen, uszPbocTag, nPbocTagLen );
				nTagsLen += nPbocTagLen;
			}

			memset( uszPbocTag, 0x0, sizeof(uszPbocTag) );
			nPbocTagLen = 0;
		}
	}

	DbgPrintHex( "需要从AFL里读取的数据的tags", szTags, nTagsLen );

	//根据tag数组进行数据读取
	if( nTagsLen )
	{
		ntmpLen = 0;
		ICC_TransReadAppDataByTag( szAFL, nAFLlen, szTags, nTagsLen, szICCardData+nICCardLen, &ntmpLen );
		nICCardLen += ntmpLen;
	}
	DbgPrintHex( "IC卡数据为", szICCardData, nICCardLen );

	//组织数据
	*pnUsrInfoLen = 0;
	for( nLoop=0; nLoop < strlen(pszTaglist); nLoop++ )
	{
		ucUserTag = pszTaglist[nLoop];
		memset( szInfo, 0x0, sizeof(szInfo) );
		nInfoLen = 0;
		if( RET_SUCCESS == IccGetUsrOrderInfo( szICCardData, nICCardLen, &ucUserTag, 1, szInfo, &nInfoLen  ) )
		{
			memcpy( pszUserInfo + (*pnUsrInfoLen) , szInfo, nInfoLen );
			(*pnUsrInfoLen) += nInfoLen;
		}
	}

	//由于df69为PDOL的数据，故直接在fci中查找，是无法获取的。修改为从PDOL中查找
	if(ICC_GetPDOL(szFci, nFciLen, szPdol, &nPdolLen) == RET_SUCCESS)
	{
		if(FindTagFromPdol("\xDF\x69", 2, szPdol, nPdolLen) == RET_SUCCESS)
		{
			nSupportGM = 1;
			DbgPrintf( "卡片支持算法选择\n" );
		}
	}

	//算法标识，从FCI中查找有无DF69
	if( 1 == IccHasDF69Tag( pszTaglist ) )
	{
		DbgPrintHex( "文件控制信息", szFci, nFciLen );
		if( nSupportGM == 0)
		{
			//卡片不支持算法选择
			DbgPrintf( "卡片不支持算法选择\n" );
			memcpy( pszUserInfo+(*pnUsrInfoLen), "Q00200", 6 );

		}
		else
		{
			//卡片支持算法选择
			DbgPrintf( "卡片支持算法选择\n" );
			memcpy( pszUserInfo+(*pnUsrInfoLen), "Q00201", 6 );
		}

		(*pnUsrInfoLen) += 6;
	}

	DbgPrintf( "szUserInfo %s\n", pszUserInfo );

GO_INIT_FAIL:      //应用初始化失败
GO_SELECT_APP_FAIL://应用选择失败
	IccPowerOff();

GO_POWER_ON_FAIL://上电失败
	/*关闭T5卡机动画*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		T5_ClosePlayDemo( *icFlag, STARTPLAYDEMO_TIMEOUT ) ;
	}
GO_STARTPLAYDEMO_FAIL://开启动画失败
	fifo_sendFinish( ) ;
	IccClosePort();

GO_OPEN_PORT_FAIL://串口打开失败
	return nRetVal;

}

int  CT_APP_GenerateARQC( char *pszComNo,int nTermType , char *pBpNo, int* icFlag, char *pszAidList, char *pszTxData, int *pnARQCLen, char *pszARQC, int nWaitCardTimeout )
{
    int nRetVal = RET_PARAM_ERR;
    unsigned char ucUserTag  = 0;
    int nLoop   = 0;
    int ntmpLen = 0;
    unsigned char szAID[64] = {0};
    int nAIDlen = 0;
    unsigned char szFci[256] = {0};
    int nFciLen = 0;
    unsigned char szAFL[256] = {0};
    int nAFLlen = 0;
    unsigned char szAIP[256] = {0};
    int nAIPlen = 0;
    unsigned char szBuff[64] = {0};
    int nBuffLen = 0;
    struct TERMDATA stTermData;
	int DetectCardStatus = 0 ;
	int iloop = 0 ;
	int retvalPowerOn = 0 ;

	if( *icFlag <1 || *icFlag>3 )
	{
		goto GO_PARAM_ERR ;
	}
	/*为了兼容喊话功能，避开卡类型 =3的情况（自动判断）*/
	if(*icFlag == 3)
	{
		*icFlag = 1 ;
	}

    //参数判断
    if(  !pszTxData || !pnARQCLen || !pszARQC )
    {
    	goto GO_PARAM_ERR ;
    }

	DbgPrintf("【NEW 生产ARQC 】 入参: pszComNo %s \n", pszComNo);

    //打开端口
	if( IccOpenPort( pszComNo, pBpNo, nTermType, BUAD_RATE, nWaitCardTimeout ) != RET_SUCCESS )
	{
		nRetVal = RET_OPEN_PORT_FAIL;
		goto GO_OPEN_PORT_FAIL;
	}

	/*开启T5卡机动画;*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		DbgPrintf("【T5_StartPlayDemo开始】 " );
		nRetVal = T5_StartPlayDemo( *icFlag , STARTPLAYDEMO_TIMEOUT ) ;
		DbgPrintf("【T5_StartPlayDemo结束】retval -- %d ", nRetVal );
		if( RET_SUCCESS != nRetVal )
		{
			fifo_sendFinish( ) ;
			IccClosePort();
			return nRetVal ;
		}
	}

	//设置超时时间;
	SetWaitCardTimeout( nWaitCardTimeout ) ;
	DbgPrintf( "超时时间g_ntimeout = %d \n", g_nWaitCardTimeout ) ;

	//上电
	nRetVal = IccPowerOn_timeout( *icFlag, NULL, NULL ) ;
	if(nRetVal != RET_SUCCESS )
	{
		DbgPrintf( "上电失败 卡类型 %d \n ", *icFlag ) ;
		//nRetVal = RET_POWER_ON_FAIL ;
		goto GO_POWER_ON_FAIL ;
	}


    //获取应用aid
	if( ( !pszAidList )   || ( strlen(pszAidList) == 0))
	{
	    //从卡片获取
        IccGetAid( szAID, &nAIDlen );
        DbgPrintHex( "从卡片获取的aid", szAID, nAIDlen );
	}
	else
    {
        //使用接口传入的参数
        nAIDlen = AsciiToHex( (unsigned char *)pszAidList, strlen(pszAidList), szAID );
        DbgPrintf("使用接口传入的AID %s\n", pszAidList );
    }

    //应用选择
    nRetVal = IccAppSelect( szAID , nAIDlen, szFci, &nFciLen );
    if( RET_SUCCESS != nRetVal && RET_CARD_LOCK != nRetVal )
    {
        DbgPrintHex("应用选择失败", szAID , nAIDlen );
        goto GO_SELECT_APP_FAIL;
    }

	//应用初始化
	memset( &stTermData, 0x0, sizeof(stTermData) );
    strncpy( stTermData.BankName, BANGK, ( strlen( BANGK ) > 20 ? 20 : strlen( BANGK ) ) );

    if( RET_SUCCESS == centerm_get_tag_value_app( (U8*)"I", 1, (U8*)pszTxData, 0, (U8*)szBuff, &nBuffLen ) )
    {
        if( !memcmp( szBuff, "01", 2 ) )
        {
            stTermData.EncryptIndex = 0x01;
        }
    }

	nRetVal = ICC_AppInitByTermData( &stTermData, szFci, nFciLen, szAFL, &nAFLlen, szAIP, &nAIPlen  );
    if( RET_SUCCESS != nRetVal )
    {
        DbgPrintHex("应用初始化", szAID , nAIDlen );
        nRetVal = RET_INIT_FAIL;
        goto  GO_INIT_FAIL;
    }

    memset( g_szICCardData, 0x0, sizeof(g_szICCardData) );
    g_nICCardLen = 0;

    //保存AIP数据
    g_szICCardData[ g_nICCardLen++ ] = 0x82;
    g_szICCardData[ g_nICCardLen++ ] = 0x02;
    g_szICCardData[ g_nICCardLen++ ] = szAIP[0];
    g_szICCardData[ g_nICCardLen++ ] = szAIP[1];

    //需要读取CDOL1,CDOL2
    ntmpLen = 0;
    ICC_TransReadCDOL( szAFL, nAFLlen, g_szICCardData+g_nICCardLen, &ntmpLen );
    g_nICCardLen += ntmpLen;
    DbgPrintHex( "IC卡数据为等待", g_szICCardData, g_nICCardLen );

    nRetVal = IccTransGenF55( BANGK, g_szICCardData, g_nICCardLen, pszTxData, pnARQCLen, pszARQC );
    if( RET_SUCCESS == nRetVal )
    {
        goto GO_SUCCESS;
    }

GO_INIT_FAIL:      //应用初始化失败
GO_SELECT_APP_FAIL://应用选择失败
GO_SUCCESS: 
	IccPowerOff();

GO_POWER_ON_FAIL://上电失败
	/*关闭T5卡机动画*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		T5_ClosePlayDemo( *icFlag, STARTPLAYDEMO_TIMEOUT ) ;
	}
GO_STARTPLAYDEMO_FAIL://开启动画失败
//成功的ARQC流程不作下电及关闭动画操作;等待写脚本流程时关闭
GO_OPEN_PORT_FAIL://串口打开失败
GO_PARAM_ERR:
	fifo_sendFinish( ) ;
    return nRetVal;

}


int  CT_APP_CloseTransAction( char *pszComNo, int nTermType, char *pBpNo, int* icFlag, char *pszTxData, char *pszARPC,
                             int *pnTCLen, char *pszTC, char *pszScriptResult )
{
    int nRetVal = RET_PARAM_ERR;
    int i   = 0;
    unsigned char szARPC[64] = {0};
    int nArpcLen = 0;
    unsigned char szAuthCode[64] = {0};
    unsigned char szBuff[64] = {0};
    int nBuffLen = 0;

    //参数判断
    if(  !pszTxData || !pszARPC  )
    {
        goto GO_PARAM_ERR ;
    }

    //打开端口
	if( IccOpenPort( pszComNo, pBpNo, nTermType, BUAD_RATE, 20 ) != RET_SUCCESS )
	{
		nRetVal = RET_OPEN_PORT_FAIL;
		goto GO_OPEN_PORT_FAIL;
	}


	/*开启T5卡机动画;*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		DbgPrintf("【T5_StartPlayDemo开始】 " );
		nRetVal = T5_StartPlayDemo( *icFlag , STARTPLAYDEMO_TIMEOUT ) ;
		DbgPrintf("【T5_StartPlayDemo结束】retval -- %d ", nRetVal );
		if( RET_SUCCESS != nRetVal )
		{
			fifo_sendFinish( ) ;
			IccClosePort();
			return nRetVal ;
		}
	}

    if( !centerm_get_tag_value_app( (unsigned char *)"T", 1, (unsigned char *)pszTxData, 0, szBuff, &nBuffLen ) )
    {
        if( !memcmp( szBuff, "51", 2 ) )
		{
			DbgPrintf("交易类型判断为解锁操作\n");
            goto GO_EXEC_SRPT;
		}

    }

	DbgPrintf( "发卡行认证 ARPC数据:%s\r\n", pszARPC );
	memset( szARPC, 0x00, sizeof(szARPC) );
	memset(szAuthCode, 0x00, sizeof(szAuthCode));
	AsciiToHex( pszARPC, strlen(pszARPC), (char *)szARPC );
	nArpcLen = strlen(pszARPC)/2;

	nRetVal = ICC_BankAuthentication( (unsigned char *)szARPC, nArpcLen, szAuthCode );
	if( nRetVal < 0 )
	{

		DbgPrintf("发卡行认证失败： %s\r\n", ICC_CardCmdShowLastErrMsg() );
		nRetVal = RET_FAIL_EXT_AUTH;
#ifdef EX_AUTH_FAIL_EXIT
		IccPowerOff();
		/*关闭T5卡机动画*/
		if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
		{
			T5_ClosePlayDemo( *icFlag, STARTPLAYDEMO_TIMEOUT ) ;
		}
		fifo_sendFinish( ) ;//发送终止标志
		IccClosePort();
		return nRetVal;
#endif
	}


	DbgPrintf( "结束交易并获取TC\n" );
	nRetVal = IccTransTC( BANGK, szAuthCode, g_szICCardData, g_nICCardLen, pszTxData, pnTCLen, pszTC );
    if( nRetVal < 0 )
	{
		DbgPrintf("结束交易失败： %s\r\n", ICC_CardCmdShowLastErrMsg() );
		nRetVal = RET_TRADE_ACC;
		goto GO_TRADE_ACC;
	}

GO_EXEC_SRPT:
    DbgPrintf( "开始执行脚本\n" );
	nRetVal = ICC_WriteScript( szARPC, nArpcLen, pszScriptResult );
	if( nRetVal < 0  && nRetVal != RET_FAIL_EXEC_SRPT )
	{
		DbgPrintf("执行脚本失败：%s\r\n", ICC_CardCmdShowLastErrMsg() );
		nRetVal = RET_FAIL_EXEC_SRPT;
		goto GO_FAIL_EXEC_SRPT;
	}


GO_TRADE_ACC://结束交易失败
GO_FAIL_EXEC_SRPT://执行脚本失败
    IccPowerOff();

	/*关闭T5卡机动画*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		T5_ClosePlayDemo( *icFlag, STARTPLAYDEMO_TIMEOUT ) ;
	}
GO_OPEN_PORT_FAIL:
GO_PARAM_ERR:
	//通过fifo发送终止标志;
	fifo_sendFinish( ) ;
    return nRetVal;

}

int  CT_APP_CtlScriptData( char *pszComNo, int nTermType , char *pBpNo, char *picFlag, char *pszTxData, char *pszARPC,
                             int *pnTCLen, char *pszTC, char *pszScriptResult )
{
	int icFlag = 0;
	return 0 ;
    //return  CT_APP_CloseTransAction( pszComNo, nTermType ,pBpNo, icFlag, pszTxData, pszARPC, pnTCLen, pszTC, pszScriptResult );
}

int  CT_APP_GetTransActionLog( char *pszComNo, int nTermType, char *pBpNo, int* icFlag, char *pszAidList, int *pnTxDetailLen, char *szTxDetail, int nWaitCardTimeout )
{
    int nRetVal = RET_PARAM_ERR;
    int nLoop   = 0;
    int ntmpLen = 0;
    unsigned char szAID[64] = {0};
    int nAIDlen = 0;
    unsigned char szFci[256] = {0};
    int nFciLen = 0;
    unsigned char bSfi = 0;
    unsigned char bNum = 0;
    unsigned char bRecordID = 0;
    unsigned char szLog[256] = {0};
    int nLogLen = 0;
    int nLogTotalLen = 0;
    char szLogTotal[1600] = {0};
    int nLogs = 0;
	int DetectCardStatus = 0 ;
	int iloop = 0 ;
	int retvalPowerOn = 0 ;

	if( *icFlag <1 || *icFlag>3 )
	{
		goto GO_PARAM_ERR ;
	}
	/*为了兼容喊话功能，避开卡类型 =3的情况（自动判断）*/
	if(*icFlag == 3)
	{
		*icFlag = 1 ;
	}

    //参数判断
    if( !pnTxDetailLen || !szTxDetail )
    {
        goto GO_PARAM_ERR ;
    }
    
    *pnTxDetailLen = 0;
    DbgPrintf("【NEW 获取交易日志】 入参：串口 %s   IC卡类型%d\n", pszComNo, *icFlag );
    //打开端口(在buf方式下，fifo_open不作任何操作，IccOpenPort主要是兼容其他方式，保存全局参数)
	if( IccOpenPort( pszComNo, pBpNo, nTermType, BUAD_RATE, nWaitCardTimeout )  != RET_SUCCESS )
	{
		//nRetVal = RET_OPEN_PORT_FAIL;
		goto GO_OPEN_PORT_FAIL;
	}

	/*开启T5卡机动画;*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		DbgPrintf("【T5_StartPlayDemo开始】 " );
		nRetVal = T5_StartPlayDemo( *icFlag , STARTPLAYDEMO_TIMEOUT ) ;
		DbgPrintf("【T5_StartPlayDemo结束】retval -- %d ", nRetVal );
		if( RET_SUCCESS != nRetVal )
		{
			goto GO_STARTPLAYDEMO_FAIL ;
		}
	}

	//设置超时时间;
	SetWaitCardTimeout(nWaitCardTimeout) ;
	DbgPrintf( "超时时间g_ntimeout = %d \n", g_nWaitCardTimeout ) ;

	//上电
	nRetVal = IccPowerOn_timeout( *icFlag, NULL, NULL ) ;
	if(nRetVal != RET_SUCCESS )
	{
		DbgPrintf( "上电失败 卡类型 %d \n ", *icFlag ) ;
		nRetVal = RET_POWER_ON_FAIL ;
		goto GO_POWER_ON_FAIL ;
	}

    //获取应用aid
	if( ( !pszAidList )   || ( strlen(pszAidList) == 0))
	{
	    //从卡片获取
        IccGetAid( szAID, &nAIDlen );
        DbgPrintHex( "从卡片获取的aid", szAID, nAIDlen );
	}
	else
    {
        //使用接口传入的参数
        nAIDlen = AsciiToHex( (unsigned char *)pszAidList, strlen(pszAidList), szAID );
        DbgPrintf("使用接口传入的AID %s\n", pszAidList );
    }

    //应用选择
    nRetVal = IccAppSelect( szAID , nAIDlen, szFci, &nFciLen );
    if( RET_SUCCESS != nRetVal && RET_CARD_LOCK != nRetVal )
    {
        DbgPrintHex("应用选择失败", szAID , nAIDlen );
        goto GO_SELECT_APP_FAIL;
    }

    //获取日志入口
	nRetVal = ICC_GetLogEntry( szFci, nFciLen, &bSfi, &bNum );
	if ( nRetVal != RET_SUCCESS )
	{
		nRetVal = RET_LOG_NO_ENTRY;
		goto GO_LOG_NO_ENTRY;
	}

    //循环读日志信息
	for( bRecordID = 1; bRecordID <= bNum; bRecordID++ )
	{
		memset( szLog, 0x0, sizeof(szLog) );
        nLogLen = 0;

		if ( ICC_GetLog( bSfi, bRecordID, szLog, &nLogLen ) )
		{
			break;
		}
		
		nLogs ++;//记录日志条数
		nLogTotalLen = IccOrgTransLog( szLogTotal, szLog );
	}
	
	//日志条数
	sprintf( szTxDetail, "%02d", nLogs );
	//每条日志长度
	if ( nLogs != 0)
	{
		sprintf( szTxDetail+2, "%03d", nLogTotalLen/nLogs );
		memcpy( szTxDetail+5, szLogTotal, nLogTotalLen );
		*pnTxDetailLen = nLogTotalLen+5;
	}
	else
	{
		sprintf( szTxDetail+2, "000" );
		*pnTxDetailLen = 0;
	}
	
    
    DbgPrintf("日志 = %s\n", szTxDetail );

GO_LOG_NO_ENTRY://没有日志入口
GO_SELECT_APP_FAIL://应用选择失败
    IccPowerOff();
GO_POWER_ON_FAIL://上电失败
	/*关闭T5卡机动画*/
	if( 0 == strcmp( "1001",pszComNo) || 0 == strcmp( "BUF",pszComNo) )
	{
		T5_ClosePlayDemo( *icFlag, STARTPLAYDEMO_TIMEOUT ) ;
	}
GO_STARTPLAYDEMO_FAIL://开启动画失败
GO_OPEN_PORT_FAIL:
GO_PARAM_ERR:
	//通过fifo发送终止标志;
	fifo_sendFinish( ) ;
    return nRetVal;

}




/*
int  CT_APP_CBKTest(char* pszComNo, int nTermType, char* pBpNo )
{
	int nRetVal = -1 ;
	//打开端口
	if( IccOpenPort(  pszComNo, pBpNo, nTermType, BUAD_RATE ) != RET_SUCCESS)
	{
		nRetVal = RET_OPEN_PORT_FAIL;
		goto GO_OPEN_PORT_FAIL;
	}

	if(T5_GatingCKB(g_ntimeout) == RET_SUCCESS ) 
	{
		nRetVal = RET_SUCCESS ;
	}
	T5_UnGatingCKB(g_ntimeout) ;
	IccClosePort();

GO_OPEN_PORT_FAIL://串口打开失败
	return nRetVal;	
}
*/
/*************************************************************************/

int IccCheckCardIsInSlot( int nSlot)
{
	
	int nReturn = -1  ;
	nReturn = ICC_CardCmdCheckCardIsInSlot( nSlot );
	if( RET_SUCCESS == nReturn )
	{
		ICC_CardReaderSetCardSlot( nSlot );
		nReturn = ICC_CardReaderGetRecStatus( );
		if ( nReturn == 0 )
		{
			nReturn = NOTOUCH_CARD_IN_UP;
		}
		else if( nReturn == -1003 )
		{
			nReturn = TOUCH_CARD_IN_UP;//接触在位已经上电
		}
		else if( nReturn == -1004 )
		{
			nReturn = TOUCH_CARD_IN;//接触在位未上电
		}
		else
		{
			nReturn = RET_OTHER_ERR;// 未知错误
		}
	}
	else
	{
		nReturn = ICC_CardReaderGetRecStatus( );
		if( nReturn != -3 )//超时
		{
			nReturn = RET_OTHER_ERR;
		}
	}
	return nReturn ;
}

int T5_StartPlayDemo(int nMode, int nTimeout)
{
	int nRetVal  = -1 ;
	int wRetVal =  -1, rRetVal = -1 ;
	int wLen = -1 , rLen = -1 ;
	U8 szCommand[ PACKET_LEN ] = { 0 } ;
	U8 szOutBuf[ PACKET_LEN ] = { 0 } ;
	wLen = sizeof(szTempletStart) ;

	memcpy( szCommand, szTempletStart, wLen ) ;
	szCommand[START_MODE_INDEX] = nMode + 0x30 ;

	DbgPrintHex("【T5--发送播放T5卡机动画指令】", szCommand, wLen ) ;
	wRetVal = fifo_write(szCommand, wLen ) ;
	if( wRetVal != wLen )
	{
		DbgPrintf("【T5--发送播放T5卡机动画指令】写错误 \n");
		return T5_STARTPLAYDEMO_ERROR ;
	}

	rRetVal = fifo_read( (U8 *)szOutBuf, 3, nTimeout );
	if(rRetVal <= 0 ) 
	{
		DbgPrintf("【T5--发送播放T5卡机动画指令】读错误 \n");
		return T5_STARTPLAYDEMO_ERROR ;
	}
	DbgPrintHex("【T5--发送播放T5卡机动画指令  --- 响应报文】", szOutBuf, rRetVal ) ;
	if((szOutBuf[0] == 0x02) && (szOutBuf[1] == 0xAA)  && (szOutBuf[2] == 0x03) )
	{
		return RET_SUCCESS ;
	}

	return T5_STARTPLAYDEMO_ERROR ;
}

int T5_ClosePlayDemo(int nMode, int nTimeout)
{
	int nRetVal  = -1 ;
	int wRetVal =  -1, rRetVal = -1 ;
	int wLen = -1 , rLen = -1 ;
	U8 szCommand[ PACKET_LEN ] = { 0 } ;
	U8 szOutBuf[ PACKET_LEN ] = { 0 } ;
	wLen = sizeof(szTempletClose) ;

	memcpy( szCommand, szTempletClose, wLen ) ;
	szCommand[CLOSE_MODE_INDEX] = nMode + 0x30 ;

	DbgPrintHex("【T5--发送关闭T5卡机动画指令】", szCommand, wLen ) ;
	wRetVal = fifo_write(szCommand, wLen ) ;
	if( wRetVal != wLen )
	{
		DbgPrintf("【T5演示程序】--------关闭T5卡机动画失败!!   写错误--------- \n");
		return T5_CLOSEPLAYDEMO_ERROR ;
	}
	
	//注：关闭卡机动画时收不到响应，不必再读取响应;

	return RET_SUCCESS ;

}

/*
int T5_GatingCKB( int nTimeout)
{
	int nRetval = -1 ;
	int wRetVal = -1 ,rRetVal = -1 ;
	U8 recBuf[PACKET_LEN] = { 0} ;
	wRetVal = ICC_WriteWindowsHid(szGatingCBK, szGatingCBKLen ) ;
	if(wRetVal != szGatingCBKLen )
	{
		return T5_GATINGCKB_ERROR ;
	}
	rRetVal = ICC_ReadWindowsHid(recBuf, 6, nTimeout, DefaultEndCondition) ;
	if( rRetVal <= 0 )
	{
		return T5_GATINGCKB_ERROR ;
	}
	if((recBuf[0] == 0x02) && (recBuf[1] == 0xAA)  && (recBuf[2] == 0x03) )
	{
		return RET_SUCCESS ;
	}

	return T5_GATINGCKB_ERROR ;

}

int T5_UnGatingCKB( int nTimeout)
{
	int nRetval = -1 ;
	int wRetVal = -1 ,rRetVal = -1 ;
	U8 recBuf[PACKET_LEN] = { 0} ;
	wRetVal = ICC_WriteWindowsHid(szGatingCBK, szGatingCBKLen ) ;
	if(wRetVal != szGatingCBKLen )
	{
		return T5_GATINGCKB_ERROR ;
	}
	return RET_SUCCESS ;
}
*/
