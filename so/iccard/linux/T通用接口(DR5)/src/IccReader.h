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

#define  NOTOUCH_CARD_IN_UP                 1     //����λδ�ϵ�
#define  TOUCH_CARD_IN_UP                   2     //����λ���ϵ�
#define  TOUCH_CARD_IN                      3


#define  RET_PARAM_ERR                      -1 //��������           
#define  RET_OPEN_PORT_FAIL                 -2 //�򿪴��ڣ�U�ڣ�ʧ��        
#define  RET_TIMEOUT_ERR                    -3//������ʱ            

#define  RET_OTHER_ERR                      -100//δ֪          
#define  RET_POWER_ON_FAIL                  -101//�ϵ�ʧ��          

#define  RET_GET_APP_FAIL                   -201//ѡ��Ӧ�û���ʧ��      
#define  RET_SELECT_APP_FAIL                -202//ѡ��Ӧ��ʧ��          
#define  RET_INIT_FAIL                      -203//Ӧ�ó�ʼ��ʧ��            
#define  RET_GEN_ARQC_FAIL                  -204//��ȡARQCʧ��          
#define  RET_FAIL_EXT_AUTH                  -205//�ⲿ��֤����          
#define  RET_FAIL_EXEC_SRPT                 -206//ִ�нű�����          
#define  RET_NOLOGENTER_ERR                 -207//û����־���
#define  RET_CARD_LOCK                      -208//��Ƭ������
#define  RET_CARD_LOCK_FOREVER              -209//��Ƭ������

#define T5_STARTPLAYDEMO_ERROR -301				//��������ʧ��
#define T5_CLOSEPLAYDEMO_ERROR -302				//�رն���ʧ��


#ifdef __cplusplus
extern "C" {
#endif


/*
@function 	CT_GetIccInfo
@intput:
		    iIcFlag  	������
			pszAidList	Ӧ���б�����
			pszTaglist	��Ҫ��ȡ�Ŀ���Ϣ��TAG����
			ntimeout	ʱ�ӣ�Ĭ��20	
@output: 			
			pszUserInfo	����Ϣ,��TLV��ʽ���
				
*/
int CT_GetIccInfo(int iIcFlag, char* pszAidList, char *pszTaglist, int* nInfoLen, char *pszUserInfo, int ntimeout) ;


/*
@function 	CT_GenerateARQC
@intput:
		    iIcFlag  	������
			pszAidList	Ӧ���б�����
			pszTxData	����ARQC�����ݣ����������ǩ����
			ntimeout	ʱ�ӣ�Ĭ��20	
@output: 			
			pszARQC		�����淶��Ҫ��55��������ASCII���ʾ
				
*/
int CT_GenerateARQC(int iIcFlag, char* pszAidList, char *pszTxData, int* nArqcLen, char *pszARQC, int ntimeout)			

/*
@function 	CT_ExeICScript
@intput:    
			iIcFlag  	������
			pszTxData	����ARQC�����ݣ����������ǩ����
			pszARPC		��̨���ص������淶��Ҫ��55��������ASCII���ʾ
			iStatus   	��̨���ص�����39�����ݽ��
			pszARQC		�����淶��Ҫ��55��������ASCII���ʾ
			ntimeout	ʱ�ӣ�Ĭ��20		
*/
int CT_ExeICScript(int iIcFlag, char *pszTxData, char *pszARPC,int iStatus,char*pszARQC ,int ntimeout��char *pszScriptResult, int* nTClen, char *pszTC )


/*
@function 	CT_GetTxDetail
@intput:
		    iIcFlag  	������
			pszAidList	Ӧ���б�����
			pszTaglist	��Ҫ��ȡ�Ŀ���Ϣ��TAG����
			ntimeout	ʱ�ӣ�Ĭ��20	
@output: 			
			pszUserInfo	����Ϣ,��TLV��ʽ���
				
*/
int CT_GetTxDetail(int iIcFlag, char*pszAidList, int ntimeout, int* nTxDetailLen, char*pszTxDetail) ;

int CT_getCardRdWrtCap() ;

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif  





#endif