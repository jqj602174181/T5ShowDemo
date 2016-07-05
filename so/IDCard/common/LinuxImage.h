#ifndef _LINUX_IMAGE_H_
#define _LINUX_IMAGE_H_

#include <stdio.h>
#include <stdlib.h>

/************************************************************************************
* 函数名：GetPhotoBmp
*  
* 功能：  将wlt文件转为bmp文件
*  
*参数：   szSoPath-----so路径
*         szWltData----Wlt图片数据
*         nWltDataLen  ----Wlt图片数据长度
*		  szImgFile[in]--图片保存完整路径
*  
*返回值： 1---成功
*         0--失败
************************************************************************************/
int GetPhotoBmp( char *szSoPath, char* szBmpData, char* szImgFile );

#endif