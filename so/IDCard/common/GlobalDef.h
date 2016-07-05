#ifndef _GLOBAL_DEF_H_
#define _GLOBAL_DEF_H_

#include "PrjDef.h"

#if defined(WINDOWS_EDITION)
#define _STDOUT _stdcall 
#include <windows.h>
#else
#define _STDOUT __attribute__  ((visibility("default"))) 
#endif


/*!<data type define */
typedef unsigned char         U8;
typedef char                  S8;
typedef unsigned int          U32;
typedef int                   S32;
typedef S32 ( *EndCondition )( U8 *, S32 );

#ifndef WORD
typedef unsigned short 	 WORD;
#endif

#ifndef BOOL
typedef int                 BOOL;
#endif

#ifndef DWORD
typedef unsigned long       DWORD;
#endif

#ifndef BYTE
typedef unsigned char       BYTE;
#endif

#ifndef TRUE
#define TRUE  (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


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
#define TYPE_SDT                          (0x3)   //sdt
#define TYPE_AUX                          (0x4)   //aux
#define TYPE_BUF                          (0x5)
/*!<define some flags */
#define TERM_LOCK                        (0x1)
#define TERM_UNLOCK                      !TERM_LOCK
#define TERM_INIT_VALUE                  (0xAA)
#define DEFAULT_TIME_OUT                 (10)
#define DEFAULT_BAUD                     (9600)


/*!<define card reader types */
#define READER_CKB51XX                   (0x0)


/*!<define HID struct types*/
typedef struct ST_HID
{
    int pid;
    int vid;
}HID;


typedef struct 
{
  S32 nInitFlag;/*!< termparam init flag*/

  S32 nTransType;/*!< 通信类型*/
  S32 nCardReader;/*!< 读卡器类型*/
  
  S32 nPID;
  S32 nVID;
  
  S32 nCom;/*!<串口号*/
  S32 nBaud;/*!<串口波特率*/
  
  #if defined( LINUX_EDITION )
  char szCom[128];
  S32 nAuxNo;/*!<辅口号*/ 
  S32 nTermType;/*!<终端类型*/  
  S32 nLockKeyboard;/*!<是否锁键盘*/
  #endif
  
  S32 nTimeOut;/*!<超时时间设置*/
  EndCondition lpEndCondition;/*!<结束条件判断*/

  //R20006 add by sulin 全局变量添加
  U8 szTermData[1024];          //形成Tc需要的初始化数据
  S32 nTermDataLen;
  
}TERMPARAM;

#endif


