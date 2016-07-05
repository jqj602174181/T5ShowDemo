/******************************************************************************************
* 
*  Copyright(C), 2002-2013, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName: PrjDef.h
*
*  Author:  quexinsheng
*
*  Date:   2014/1/7 14:47:01
*
*  Description: 项目自定义
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
/*!<
           通信方式说明
目前，PBOC2.0v1.10的代码支持的通信方式有
window环境有：
COM_WIN_MODE         串口通信方式
HID_WIN_MODE         hid通信方式
PCSC_WIN_MODE        smartcard通信方式
Linux环境有：        
COM_LINUX_MODE       串口通信方式
这些方法通过_LINUX_宏控制，未定义了则是window环境
*/

#ifndef _SELF_DEF_H_
#define _SELF_DEF_H_

#define USE_READER_CKB51XX
#define LINUX_EDITION
#define DEBUG_OUTPUT        
#define DEBUG_FILE          ("//mnt//sdcard//debug.log")
//#define USE_TYPE_COM_WITHOUT_AUX    //串口通信方式,无辅口
#define USE_TYPE_BUF

#define BANGK                       "XX银行"

//定义usb描述符

typedef struct ST_HID
{
	int pid;
	int vid;
}HID;//HID{pid,vid}

#define  HID_VID_01          0x0403
#define  HID_PID_01          0xBCD5


#define  HID_VID_02          0x0403
#define  HID_PID_02          0xBCD7

#define  HID_VID   HID_VID_01
#define  HID_PID   HID_PID_01

#define  BUAD_RATE            9600 //T5
#define  BP_CMD               "\x1B\x25" //转口指令
#define  ICC_PORT_25_9600     "\x1b\x25\x49\x34"
#define  ICC_DEFAULT_25_PORT  "\x1b\x25\x42"
#define  ICC_PORT_24_9600     "\x1b\x24\x49\x34"
#define  ICC_DEFAULT_24_PORT  "\x1b\x24\x42"

//定义这个宏，则默认使用A000000333，而不从卡片中获取
#define  SPEED_UP_NO_GET_AID_FROM_CARD

//定义这个宏，define main() ;
//#define  FUNCTION_TEST

#define COMNO  "BUF"   

#endif





