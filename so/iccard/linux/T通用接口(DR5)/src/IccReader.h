#ifndef  _ICC_READER_H_
#define  _ICC_READER_H_

#include"PrjDef.h"

#ifndef _ICC_READER_H_
#define _ICC_READER_H_

#ifndef LINUX_EDITION
#define _STDOUT _stdcall 
#include <windows.h>
#else
#include <stdio.h>
#define _STDOUT __attribute__  ((visibility("default"))) 
#endif

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

#define T5_STARTPLAYDEMO_ERROR -301				//开启动画失败
#define T5_CLOSEPLAYDEMO_ERROR -302				//关闭动画失败


#ifdef __cplusplus
extern "C" {
#endif


/*
@function 	CT_GetIccInfo
@intput:
		    iIcFlag  	卡类型
			pszAidList	应用列表数组
			pszTaglist	需要获取的卡信息的TAG数组
			ntimeout	时延，默认20	
@output: 			
			pszUserInfo	卡信息,以TLV格式输出
				
*/
int CT_GetIccInfo(int iIcFlag, char* pszAidList, char *pszTaglist, int* nInfoLen, char *pszUserInfo, int ntimeout) ;


/*
@function 	CT_GenerateARQC
@intput:
		    iIcFlag  	卡类型
			pszAidList	应用列表数组
			pszTxData	产生ARQC的数据，包含多个标签变量
			ntimeout	时延，默认20	
@output: 			
			pszARQC		银联规范需要的55数据域，以ASCII码表示
				
*/
int CT_GenerateARQC(int iIcFlag, char* pszAidList, char *pszTxData, int* nArqcLen, char *pszARQC, int ntimeout)			

/*
@function 	CT_ExeICScript
@intput:    
			iIcFlag  	卡类型
			pszTxData	产生ARQC的数据，包含多个标签变量
			pszARPC		后台返回的银联规范需要的55数据域，以ASCII码表示
			iStatus   	后台返回的银联39域数据结果
			pszARQC		银联规范需要的55数据域，以ASCII码表示
			ntimeout	时延，默认20		
*/
int CT_ExeICScript(int iIcFlag, char *pszTxData, char *pszARPC,int iStatus,char*pszARQC ,int ntimeout，char *pszScriptResult, int* nTClen, char *pszTC )


/*
@function 	CT_GetTxDetail
@intput:
		    iIcFlag  	卡类型
			pszAidList	应用列表数组
			pszTaglist	需要获取的卡信息的TAG数组
			ntimeout	时延，默认20	
@output: 			
			pszUserInfo	卡信息,以TLV格式输出
				
*/
int CT_GetTxDetail(int iIcFlag, char*pszAidList, int ntimeout, int* nTxDetailLen, char*pszTxDetail) ;

int CT_getCardRdWrtCap() ;

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif  





#endif