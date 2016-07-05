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
*  Description: ��Ŀ�Զ���
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
/*!<
           ͨ�ŷ�ʽ˵��
Ŀǰ��IDCard�Ĵ���֧�ֵ�ͨ�ŷ�ʽ��
window�����У�
	USE_TYPE_COM         ����ͨ�ŷ�ʽ
	USE_TYPE_HID         hidͨ�ŷ�ʽ
	USE_TYPE_SDT		 sdtͨ�ŷ�ʽ
	USE_TYPE_PCSC        smartcardͨ�ŷ�ʽ
Linux�����У�        
	USE_TYPE_COM		 ����ͨ�ŷ�ʽ
	USE_TYPE_AUX         ����ͨ�ŷ�ʽ
��Щ����ͨ��_LINUX_����ƣ�δ����������window����
*/

#ifndef _SELF_DEF_H_
#define _SELF_DEF_H_

/*!< Ҫ�����ƽ̨  */
//#define WINDOWS_EDITION	//windows����
#define  LINUX_EDITION


/*!< Ҫʹ�õ�ͨ�ŷ�ʽ  */
//#define USE_TYPE_HID
//#define USE_TYPE_SDT           
#define USE_TYPE_COM           
//#define USE_TYPE_PCSC    
#define USE_TYPE_AUX
#define USE_TYPE_BUF
 

//�򿪵���
//#define  DEBUG_OUTPUT            
#define  DEBUG_FILE    ("debug.log")

//��������
#define     TIMES_READCARD    1		//Ĭ�϶������Դ���


//ʹ�ù̶���PID��VID���з��ʣ������Ͳ�����ѭ����PID��VID
//ע�⣺�����µ�PID,VIDʱ����뵽s_HID��
#ifdef USE_TYPE_HID   
	#define  FIXED_PID    (0x0)
	#define  FIXED_VID    (0x0)
#endif


//USB�����Ƿ�����Ѱ����ѡ��,ע������ó�ʱ�ж�ʧЧ
#if  defined(USE_TYPE_HID) || defined(USE_TYPE_SDT) 
	//#define  USB_PASS_SEARCH_SELECT   
#endif


/***************���º�Ϊ���֤ͼƬ����******************
**ע�⣺ 1. ��ʽ֧�֣� Linux��֧��Bmp��ʽ��ͼƬ
**                     Window��֧��bmp,jpg, png ,gif��ʽ��ͼƬ
**       2. ͼƬ���ͣ�Linux֧��ͷ��ͼƬ
**		              Window֧��ͷ�����棬���棬������ͼƬ
******************************************************/

#ifdef WINDOWS_EDITION

	//���֤��������
	#define     FILE_WLTRS        "WltRS.dll"
	
	//���֤ͼƬ�����ļ�������
	#define     FILE_TMP_HEAD     "_centerm_idcard_tmp_head_.bmp"
	#define     FILE_TMP_IMG      "_centerm_idcard_img_.bmp"
	#define     FILE_TMP_IMG_JPG  "_centerm_idcard_img_.jpg"

	//ע�⣺���û�����������ͼƬ·��ʱ������ "FILE_IMG_XXX" �� "EXFILE_XXX" ��������Ч
	//���ͻ�ָ������ļ���,���������ļ���xxx.bmp/jpg/png/gif������Ҫʱ������Ϊ���ַ���
	#define		FILE_IMG_HEAD	  ""
	#define		FILE_IMG_FRONT	  ""
	#define		FILE_IMG_BACK	  ""
	#define		FILE_IMG_CARD	  ""

	//���ͻ�δָ������ļ���ʱ���ļ�����ʽΪ���֤��+XXX.bmp/jpg/png/gif
	#define     EXFILE_HEAD       "-head.bmp"
	#define     EXFILE_CARD       "-card.bmp"
	#define     EXFILE_BACK       "-back.bmp"
	#define     EXFILE_FRONT      "-front.bmp"
	
#else //LINUX_EDITION

	//���֤��������
	#define     FILE_WLTRS        "libidcaread.so"	
	
	//���汣��·��
	#define     FILE_TMP_DIR      "/mnt/sdcard/"	
	
		//���֤ͼƬ�����ļ�������
	#define     FILE_TMP_HEAD     "_centerm_idcard_tmp_head_.bmp"
	#define     FILE_TMP_IMG      "_centerm_idcard_img_.bmp"

	//ע�⣺���û�����������ͼƬ·��ʱ������ "FILE_IMG_XXX" �� "EXFILE_XXX" ��������Ч
	//���ͻ�ָ������ļ���,���������ļ���xxx.bmp������Ҫʱ������Ϊ���ַ���
	#define		FILE_IMG_HEAD	  ""

	//���ͻ�δָ������ļ���ʱ���ļ�����ʽΪ���֤��+XXX.bmp
	#define     EXFILE_HEAD       "-head.bmp"
	
#endif
		


#endif



