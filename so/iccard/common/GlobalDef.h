#ifndef _GLOBAL_DEF_H_
#define _GLOBAL_DEF_H_

#include "PrjDef.h"
/*!<data type define */
typedef unsigned char         U8;
typedef char                  S8;
typedef unsigned int          U32;
typedef int                   S32;
typedef S32 ( *EndCondition )( U8 *, S32 );

/*!<same useful marco define */
#define TABLESIZE(a)                                ( sizeof(a)/sizeof(a[0]) )
#define INVALID_DESCRIPTOR                          (-1)
#define MAX_LEN                                     (1024)
#define PACKET_LEN                                  (2048)
#define ICC_CMD_HAS_LE                              (0x1)
/*!<define return values */
#define RET_SUCCESS                                 (0)
#define RET_ERROR                                   (-1)
#define RET_INVALID_PARAM                           (-2)
#define RET_FAIL_OPEN_FILE                          (-3)
#define RET_FAIL_TIMEOUT                            (-4)
#define RET_WRITE_ERROR                             (-5)
#define RET_READ_ERROR                              (-6)
#define RET_DEVICE_UNOPEN                           (-7)
#define RET_WRONG_PACKET                            (-8)
#define RET_CARDSLOT_ERROR                          (-9)
#define RET_NO_FOUND_SFI                            (-10)
#define RET_SFI_LEN_ERR                             (-11)
#define RET_MALLOC_ERR                              (-12)
#define RET_DOL_NO_FOUND                            (-13)
#define RET_DOL_DATA_ERR                            (-14)
#define RET_DOL_DATA_NO_FOUND                       (-15)
#define RET_APP_INIT_ERR                            (-16)
#define RET_LOG_NO_ENTRY                            (-17)
#define RET_RECORD_NO_FOUND                         (-18)
#define RET_GET_GAC_TOO_MUCH                        (-19)
#define RET_CMD_ERR                                 (-20)
#define RET_GAC_RESP_ERR                            (-21)
#define RET_AUTH_DATA_ERROR                         (-22)
#define RET_TRADE_ACC                               (-23)
#define RET_NO_SCRIPT                               (-24)
#define RET_WRITE_SCRIPT_ERR                        (-25)
#define RET_SCRIPT_CMD_ERR                          (-26)



/*!<define Communication types */
#define TYPE_HID                          (0x0)   //hid
#define TYPE_COM                          (0x1)   //com
#define TYPE_PCSC                         (0x2)   //pc/sc
#define TYPE_BUF                          (0x3)   //fifo Buf  (add for T5) 
#define TYPE_BHT						  (0x4)   //bluetooth (add for T5)



/*!<define some flags */
#define TERM_LOCK                        (0x1)
#define TERM_UNLOCK                      !TERM_LOCK
#define TERM_INIT_VALUE                  (0xAA)
#define DEFAULT_TIME_OUT                 (10)
#define DEFAULT_BAUD                     (9600)

/*!<define card reader types */
#define READER_CKB51XX                   (0x0)



//������ 
//�Ӵ�ʽ��
#define  SMART_CARD_BASE                0x0
#define  SMART_CARD                     SMART_CARD_BASE      //�Ӵ�ʽ��
#define  SMART_CARD_MAX                 (SMART_CARD_BASE+0xF)//�Ӵ�ʽ�����Ŀ���
#define  IS_ICC_SLOT( a )               ( ( (a) >= SMART_CARD_BASE )&&( (a) <= SMART_CARD_MAX ) ? 1 : 0 )

 //�ǽӴ�ʽ��
#define PICC_CARD_SLOT                  (0xFF)

//PSAM��
#define  PSAM_CARD_BASE                 0x10
#define  PSAM_CARD                      PSAM_CARD_BASE      
#define  PSAM_CARD_MAX                  (PSAM_CARD_BASE+0xF)//PSAM�����Ŀ���
#define  IS_PSAM_SLOT( a )              ( ( (a) >= PSAM_CARD )&&( (a) <= PSAM_CARD_MAX ) ? 1 : 0 )



//GAC�������
#define CMD_GAC_ARQC    (1)
#define CMD_GAC_TC      (2)
#define CMD_GAC_AAC     (3)

typedef struct 
{
  S32 nInitFlag;/*!< termparam init flag*/

  S32 nTransType;/*!< ͨ������*/
  S32 nCardReader;/*!< ����������*/
  
  S32 nPID;
  S32 nVID;
  
  S32 nCom;/*!<���ں�*/
  S32 nBaud;/*!<���ڲ�����*/
  
  #if defined( LINUX_EDITION )
  S32 nAuxNo;/*!<���ں�*/ 
  S32 nTermType;/*!<�ն�����*/  
  S32 nLockKeyboard;/*!<�Ƿ�������*/
  S8  szTermPath[256];
  #endif
  
  S32 nTimeOut;/*!<��ʱʱ������*/
  EndCondition lpEndCondition;/*!<���������ж�*/

  //R20006 add by sulin ȫ�ֱ������
  U8 szTermData[1024];          //�γ�Tc��Ҫ�ĳ�ʼ������
  S32 nTermDataLen;
  
}TERMPARAM;

typedef struct 
{
	U8 szIccAppData[4096] ;
	U8 szAip[2+1];
	U32 nIccAppDataLen ;
	U8 szTcF55[4096];		//add by cjm
	U32 nTcF55Len;			//add by cjm
}ICCARDPARAM;


#define TAG_LEN     32
typedef struct
{
	char UsrTag[ TAG_LEN ];     //�û��ı�ǩ
	int  UsrTagLen;             //�û���ǩ����
	char OurTag[ TAG_LEN ];     //�����Լ��ı�ǩ
	int OurTagLen;              //��ǩ����
} TAGLIST;

#endif


