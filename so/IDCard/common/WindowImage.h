/******************************************************************************************
* 
*  Copyright(C), 2002-2015, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:    WindowImage.h
*
*  Author:      chenwenbin
*
*  Date:        2015-9-14
*
*  Description: windowsλͼ��������
*
*******************************************************************************************/


#ifndef BITMAPCOMMAND_H_
#define BITMAPCOMMAND_H_

#include "IDCardDef.h"


#define  DBINCHPERMETEAR   39.375

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************************
* ��������GetPhotoBmp
*  
* ���ܣ�  ͨ��WIndow�µ�dll��wlt�ļ�תΪbmp�ļ�
*  
*������   szImgFile[in]--wlt�ļ�
*  
*����ֵ�� TRUE---�ɹ�
*         FALSE--ʧ��
************************************************************************************/
BOOL GetPhotoBmp( char *szDllPath , char* szImgFile );




/*****************************************************************
��������SaveBitmapFile
������  ͨ��λͼ�����λͼ���浽����Ӳ��
������  strFileName[in]--����ı���·����bmp--λͼ���
����ֵ��TRUE-�ɹ���FALSE-ʧ��
��ע��  ��
******************************************************************/
BOOL SaveBitmapFile( const char *strFileName, HBITMAP bmp );




/*****************************************************************
��������SaveFrontImage
������  ����ȡ������֤������Ϣ���������ͼƬ
������  hModule-������Դ��ģ����, nResID-��Դ��
		personInfo-��ȡ���Ķ���֤��Ϣ��szImagePath-Ҫ���������ͼƬ·����
        szHeadImage-ͷ��ͼƬ·��
����ֵ����
��ע��  ��
******************************************************************/
BOOL SaveFrontImage(HMODULE hModule, unsigned int nResID, PERSONINFO personInfo, const char*szPhotoPath, const char *szImagePath );


/*****************************************************************
��������SavaBackimage
������  ����ȡ������֤��������Ϣ����ɷ���ͼƬ
������  hModule-������Դ��ģ����, nResID-��Դ��
		personInfo-��ȡ���Ķ���֤��Ϣ��szImagePath-Ҫ����ķ���ͼƬ·����
����ֵ����
��ע��  ��
******************************************************************/
BOOL SaveBackImage( HMODULE hModule, unsigned int nResID, PERSONINFO pInfo, const char *szImagePath );


/*****************************************************************
��������SaveIDCardImage
������  �������֤ͼƬ��������ͬһ��
������  hModule-������Դ��ģ����, nResID-��Դ��,bMakeLogo-�Ƿ�ˮӡ
		personInfo-��ȡ���Ķ���֤��Ϣ��szPhotoPath-ͷ��·����szImagePath-Ҫ����ķ���ͼƬ·����
����ֵ����
��ע��  ��
******************************************************************/
BOOL SaveIDCardImage(HMODULE hModule, unsigned int nResID,  BOOL bMakeLogo, PERSONINFO personInfo, const char* szPhotoPath, const char* szImgPath );



#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif