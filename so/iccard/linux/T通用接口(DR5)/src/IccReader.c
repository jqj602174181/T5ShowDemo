#include "GlobalDef.h"
#include "ECash.h"
#include "TransTag.h"
#include "Debug.h"
#include "IccReader.h"
#include "IccAppComm.h"
#include "CardReaderLayer.h"
#include "ICC_CardCmd.h"
#include "StrOp.h"
#include "TlvOp.h"
#include <time.h> 
#include <string.h> 
#include <sys/timeb.h>
#include <debugLog.h>
#include <stdio.h>
#include <string.h>
#include "PrjDef.h"

#include <sys/timeb.h>

#define TAG_LIST_MAXLEN 50 

#define  NOTOUCH_CARD_IN_UP                 1     //卡在位未上电
#define  TOUCH_CARD_IN_UP                   2     //卡在位已上电
#define  TOUCH_CARD_IN                      3


#define  RET_PARAM_ERR                      -1 //参数错误           
#define  RET_OPEN_PORT_FAIL                 -2 //打开串口（U口）失败        
#define  RET_TIMEOUT_ERR                    -3//读卡超时            

#define  RET_OTHER_ERR                      -100//未知          
#define  RET_POWER_ON_FAIL                  -101//上电失败          

#define  RET_GET_APP_FAIL                   -201//选择应用环境失败      
#define  RET_SELECT_APP_FAIL                -202//选择应用失败          
#define  RET_INIT_FAIL                      -203//应用初始化失败            
#define  RET_GEN_ARQC_FAIL                  -204//获取ARQC失败          
#define  RET_FAIL_EXT_AUTH                  -205//外部认证错误          
#define  RET_FAIL_EXEC_SRPT                 -206//执行脚本错误          
#define  RET_NOLOGENTER_ERR                 -207//没有日志入口
#define  RET_CARD_LOCK                      -208//卡片被锁定
#define  RET_CARD_LOCK_FOREVER              -209//卡片被锁定

/*T5新增*/
#define T5_STARTPLAYDEMO_ERROR				-301//T5开启卡机动画失败    
#define T5_CLOSEPLAYDEMO_ERROR				-302//T5关闭卡机动画失败 



char* getTime( void )
{
    struct  tm  *ptm;
    struct  timeb stTimeb;
    static  char  szTime[128];
   
    memset( szTime, 0x0, sizeof(szTime) );
    ftime( &stTimeb );
    ptm = localtime( &stTimeb.time );
    
    sprintf( szTime, "%02d%02d%02d%02d%02d%03d", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm );
    
    return szTime;
}

void  CT_SetSlot( int nSlot)
{
	CT_App_SetSlot( nSlot );
}

/*
int CT_GetCardStatus(int nComNo)
{
    return CT_APPGetCardStatus( nComNo, 10 );
}
*/

int CT_GetIccInfo(int iIcFlag, char* pszAidList, char *pszTaglist, int* nUserInfo, char *pszUserInfo, int ntimeout)
{
	int nRetVal = -1 ;
	int icType = 0 ;
	icType = iIcFlag ;
    
	nRetVal = CT_APP_GetIccInfo( COMNO, 1, NULL,  &icType, pszAidList, pszTaglist, nUserInfo, pszUserInfo, ntimeout) ;


	return nRetVal ;
}

int CT_GenerateARQC(int iIcFlag, char* pszAidList, char *pszTxData, int* nArqcLen, char *pszARQC, int ntimeout)
{
	int nRetVal = -1 ;
	int icType = 0 ;
	icType = iIcFlag ;


	nRetVal = CT_APP_GenerateARQC( COMNO, 1, NULL, &icType, pszAidList, pszTxData, nArqcLen, pszARQC, ntimeout) ;

	return nRetVal ;
}

int CT_ExeICScript(int iIcFlag, char *pszTxData, char *pszARPC,int iStatus,char*pszARQC ,int ntimeout,char *pszScriptResult, int* nTClen, char *pszTC )
{
	int nRetVal = -1 ;
	int icType = 0 ;
	icType = iIcFlag ;

	nRetVal = CT_APP_CloseTransAction(COMNO, 1, NULL, &icType ,pszTxData, pszARPC,nTClen, pszTC, pszScriptResult ) ;		
	return nRetVal ;
}

int CT_GetTxDetail(int iIcFlag, char*pszAidList, int ntimeout, int* nTxDetailLen, char*pszTxDetail)
{
	int nRetVal = -1 ;
	int icType = 0 ;
	icType = iIcFlag ;

	nRetVal = CT_APP_GetTransActionLog( COMNO, 1, NULL, &icType, pszAidList, nTxDetailLen, pszTxDetail, ntimeout ) ;	

	return nRetVal ;
}

int CT_getCardRdWrtCap() 
{
	return 6 ; //6表示设备支持接触非接方式;
}



