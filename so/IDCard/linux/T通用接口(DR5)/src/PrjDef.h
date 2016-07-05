/******************************************************************************************
* 
*  Copyright(C), 2002-2013, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName: PrjDef.h
*
*  Author:  chenwenbin
*
*  Date:   2015/7/24 15:13:01
*
*  Description: 项目自定义
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
/*!<
           通信方式说明
目前，IDCard的代码支持的通信方式有
window环境有：
	USE_TYPE_COM         串口通信方式
	USE_TYPE_HID         hid通信方式
	USE_TYPE_SDT		 sdt通信方式
	USE_TYPE_PCSC        smartcard通信方式
Linux环境有：        
	USE_TYPE_COM		 串口通信方式
	USE_TYPE_AUX         辅口通信方式
这些方法通过_LINUX_宏控制，未定义了则是window环境
*/

#ifndef _SELF_DEF_H_
#define _SELF_DEF_H_

/*!< 要编译的平台  */
//#define WINDOWS_EDITION	//windows驱动
#define  LINUX_EDITION


/*!< 要使用的通信方式  */
//#define USE_TYPE_HID
//#define USE_TYPE_SDT           
#define USE_TYPE_COM           
//#define USE_TYPE_PCSC    
#define USE_TYPE_AUX
#define USE_TYPE_BUF
 

//打开调试
//#define  DEBUG_OUTPUT            
#define  DEBUG_FILE    ("debug.log")

//常量设置
#define     TIMES_READCARD    1		//默认读卡尝试次数


//使用固定的PID、VID进行访问，这样就不会轮循已有PID、VID
//注意：当有新的PID,VID时请加入到s_HID中
#ifdef USE_TYPE_HID   
	#define  FIXED_PID    (0x0)
	#define  FIXED_VID    (0x0)
#endif


//USB读卡是否跳过寻卡与选卡,注意这会让超时判断失效
#if  defined(USE_TYPE_HID) || defined(USE_TYPE_SDT) 
	//#define  USB_PASS_SEARCH_SELECT   
#endif


/***************以下宏为身份证图片设置******************
**注意： 1. 格式支持： Linux下支持Bmp格式的图片
**                     Window下支持bmp,jpg, png ,gif格式的图片
**       2. 图片类型：Linux支持头像图片
**		              Window支持头像，正面，反面，正反面图片
******************************************************/

#ifdef WINDOWS_EDITION

	//身份证解析库名
	#define     FILE_WLTRS        "WltRS.dll"
	
	//身份证图片缓存文件名设置
	#define     FILE_TMP_HEAD     "_centerm_idcard_tmp_head_.bmp"
	#define     FILE_TMP_IMG      "_centerm_idcard_img_.bmp"
	#define     FILE_TMP_IMG_JPG  "_centerm_idcard_img_.jpg"

	//注意：当用户传入完整的图片路径时，以下 "FILE_IMG_XXX" 与 "EXFILE_XXX" 宏设置无效
	//当客户指定输出文件名,设置下列文件名xxx.bmp/jpg/png/gif，不需要时请设置为空字符串
	#define		FILE_IMG_HEAD	  ""
	#define		FILE_IMG_FRONT	  ""
	#define		FILE_IMG_BACK	  ""
	#define		FILE_IMG_CARD	  ""

	//当客户未指定输出文件名时，文件名格式为身份证号+XXX.bmp/jpg/png/gif
	#define     EXFILE_HEAD       "-head.bmp"
	#define     EXFILE_CARD       "-card.bmp"
	#define     EXFILE_BACK       "-back.bmp"
	#define     EXFILE_FRONT      "-front.bmp"
	
#else //LINUX_EDITION

	//身份证解析库名
	#define     FILE_WLTRS        "libidcaread.so"	
	
	//缓存保存路径
	#define     FILE_TMP_DIR      "/mnt/sdcard/"	
	
		//身份证图片缓存文件名设置
	#define     FILE_TMP_HEAD     "_centerm_idcard_tmp_head_.bmp"
	#define     FILE_TMP_IMG      "_centerm_idcard_img_.bmp"

	//注意：当用户传入完整的图片路径时，以下 "FILE_IMG_XXX" 与 "EXFILE_XXX" 宏设置无效
	//当客户指定输出文件名,设置下列文件名xxx.bmp，不需要时请设置为空字符串
	#define		FILE_IMG_HEAD	  ""

	//当客户未指定输出文件名时，文件名格式为身份证号+XXX.bmp
	#define     EXFILE_HEAD       "-head.bmp"
	
#endif
		


#endif



