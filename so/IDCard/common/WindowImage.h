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
*  Description: windows位图操作函数
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
* 函数名：GetPhotoBmp
*  
* 功能：  通过WIndow下的dll将wlt文件转为bmp文件
*  
*参数：   szImgFile[in]--wlt文件
*  
*返回值： TRUE---成功
*         FALSE--失败
************************************************************************************/
BOOL GetPhotoBmp( char *szDllPath , char* szImgFile );




/*****************************************************************
函数名：SaveBitmapFile
描述：  通过位图句柄将位图保存到本地硬盘
参数：  strFileName[in]--保存的本地路径，bmp--位图句柄
返回值：TRUE-成功，FALSE-失败
备注：  无
******************************************************************/
BOOL SaveBitmapFile( const char *strFileName, HBITMAP bmp );




/*****************************************************************
函数名：SaveFrontImage
描述：  将读取到二代证正面信息保存成正面图片
参数：  hModule-包含资源的模块句柄, nResID-资源号
		personInfo-读取到的二代证信息，szImagePath-要保存的正面图片路径，
        szHeadImage-头像图片路径
返回值：无
备注：  无
******************************************************************/
BOOL SaveFrontImage(HMODULE hModule, unsigned int nResID, PERSONINFO personInfo, const char*szPhotoPath, const char *szImagePath );


/*****************************************************************
函数名：SavaBackimage
描述：  将读取到二代证反面面信息保存成反面图片
参数：  hModule-包含资源的模块句柄, nResID-资源号
		personInfo-读取到的二代证信息，szImagePath-要保存的反面图片路径，
返回值：无
备注：  无
******************************************************************/
BOOL SaveBackImage( HMODULE hModule, unsigned int nResID, PERSONINFO pInfo, const char *szImagePath );


/*****************************************************************
函数名：SaveIDCardImage
描述：  保存身份证图片，正反面同一张
参数：  hModule-包含资源的模块句柄, nResID-资源号,bMakeLogo-是否画水印
		personInfo-读取到的二代证信息，szPhotoPath-头像路径，szImagePath-要保存的反面图片路径，
返回值：无
备注：  无
******************************************************************/
BOOL SaveIDCardImage(HMODULE hModule, unsigned int nResID,  BOOL bMakeLogo, PERSONINFO personInfo, const char* szPhotoPath, const char* szImgPath );



#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif