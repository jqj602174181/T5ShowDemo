#ifndef _ICCAPPCOMM_H_H
#define _ICCAPPCOMM_H_H

#define  NOTOUCH_CARD_IN_UP        1							    //卡在位未上电
#define  TOUCH_CARD_IN_UP          2								//卡在位已上电
#define  TOUCH_CARD_IN             3
static int g_nWaitCardTimeout = 20 ;
#define STARTPLAYDEMO_TIMEOUT 5

void CT_App_SetSlot(int nSlot);
/*****************************************************************************************************************************/

int  CT_APP_GetIccInfo( char *pszComNo, int nTermType,  char *pBpNo, int* icFlag, char *pszAidList, char *pszTaglist, int *pnUsrInfoLen, char *pszUserInfo, int nWaitCardTimeout ) ;
int  CT_APP_GenerateARQC( char *pszComNo, int nTermType,  char *pBpNo, int* icFlag, char *pszAidList, char *pszTxData, int *pnARQCLen, char *pszARQC , int nWaitCardTimeout );
int  CT_APP_CloseTransAction( char *pszComNo, int nTermType, char *BpNo, int* icFlag,char *pszTxData, char *pszARPC,
	int *pnTCLen, char *pszTC, char *pszScriptResult );
int  CT_APP_GetTransActionLog( char *pszComNo, int nTermType , char *BpNo,int* icFlag,char *pszAidList,int *pnTxDetailLen, char *TxDetail, int nWaitCardTimeout );
int  CT_APP_GetCreditLoadLog( char *pszComNo, int nTermType , char *pBpNo,char *picFlag, char *pszAidList, int *pnTxDetailLen, char *szTxDetail );
int  CT_APP_CBKTest(char* pszComNo, int nTermType, char* pBpNo ) ;

/********************************************************************/
//T5相关


#include "read_fifo.h"


#define T5_STARTPLAYDEMO_ERROR -301
#define T5_CLOSEPLAYDEMO_ERROR -302

static U8 szTempletStart[] = { 0x1b, 0x5b, 0x38, 0x4f, 0x50, 0x45, 0x4e, 0x43, 0x48, 0x41, 0x4e, 0x4e, 0x45, 0x4c, 0x02, 0x32, 0x03  } ; //17
static U8 szTempletClose[] = { 0x1b, 0x5b, 0x38, 0x43, 0x4c, 0x4f, 0x53, 0x45, 0x43, 0x48, 0x41, 0x4e, 0x4e, 0x45, 0x4c, 0x02, 0x32, 0x03 } ;//18
#define START_MODE_INDEX 15
#define CLOSE_MODE_INDEX 16


#define  ETX  0x03  
//终止条件
static S32 DefaultEndCondition( U8 *szData, S32 nDataLen )
{
	if( ETX == szData[ nDataLen-1 ] )
	{
		return RET_SUCCESS;    
	}

	return RET_ERROR;

}

/************************************************************************/
/*  T5_StartPlayDemo 播放卡机动画                                                       
/*   nMode   0 1 2 3 4 5   身份证，接触，非接，磁条卡，指纹仪
/*   nTimeout
/*   return: 0  成功
             -1 失败
			 T5_OPENHID_ERROR -301
			 T5_STARTPLAYDEMO_ERROR -302
/************************************************************************/
int T5_StartPlayDemo(int nMode, int nTimeout);
/************************************************************************/
/*  T5_ClosePlayDemo 关闭卡机动画                                                       
/*   nMode   0 1 2 3 4 5   身份证，接触，非接，磁条卡，指纹仪
/*   nTimeout
/*   return: 0  成功
             -1 失败
			 T5_OPENHID_ERROR -301
			 T5_STARTPLAYDEMO_ERROR -302
/************************************************************************/
int T5_ClosePlayDemo(int nMode, int nTimeout);

int IccCheckCardIsInSlot( int nSlot) ;


#endif



