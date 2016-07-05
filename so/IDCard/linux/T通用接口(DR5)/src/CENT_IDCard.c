// CENT_IDCard.c : 定义 SO 应用程序的导出函数。
//
#include "PrjDef.h"
#include <string.h>
#include "CENT_IDCard.h"
#include "IDCard.h"
#include "Debug.h"
#include <stdio.h> //C++
#include "Common.h" //C++



long  CT_SaveHeadImg( char* cardId, char* szHeadImg )
{
	char szHeadBmp[ 260 ] = { 0 };
	char szImgPath[ 260 ] = { 0 };
	char *pHeadImg = szHeadBmp;

	//设置缓存路径
	sprintf( szHeadBmp,"%s%s",FILE_TMP_DIR,FILE_TMP_HEAD);
	
	if (szHeadImg != NULL)
	{
		//如果szHeadImg有空间但无长度，则赋值为默认的身份证图片路径
		if(strlen(szHeadImg) <= 0)
		{
			char *pTmp = strrchr(szHeadBmp, '/' ) + 1;
			strncpy(szHeadImg, szHeadBmp, pTmp - szHeadBmp);
		}
		pHeadImg = szHeadImg;
	}

	//设置保存路径
	if(strlen(FILE_IMG_HEAD) > 0)
		GetImgPathEx( pHeadImg, "", FILE_IMG_HEAD, szImgPath, sizeof(szImgPath)-1 );
	else
		GetImgPathEx( pHeadImg, EXFILE_HEAD, cardId, szImgPath, sizeof(szImgPath)-1 );

	//保存图片
	if( szHeadImg == NULL || CopyFile( szHeadBmp, szImgPath, TRUE ) )
	{
		if( szHeadImg != NULL )
			strcpy( szHeadImg, szImgPath );
		return ERR_SUCCESS;
	}
	else
	{
		return ERR_SAVEIMG;
	}
}


/************************************************************************************
* 函数名：CT_GetHeadImg
*  
* 功能： 获取二代证头像
*  
*参数： 
*         szHeadDir[in]--头像保存目录，取值为NULL或“”时取消保存图片
*         szData[in]----二代证原始信息
*		  cardId[in] ---二代证身份证号
*  
*返回值： ERR_SUCCESS---成功
*         ERR_DEVICE---设备错误
*         ERR_SEND-----发送错误
*         ERR_RECV-----接收错误
*         ERR_CHECK-----校验错误
*         ERR_TIMEOUT--超时
*         ERR_READ-----读卡错误
*         ERR_IMAGE-----解析头像错误
*         ERR_SAVEIMG-----保存图像错误
*		  ERR_CANCEL-----取消操作
*         ERR_OTHER-----其他错误
************************************************************************************/
long _STDOUT CT_GetHeadImg   ( char* szHeadDir, char* szData ,char* cardId)
{
	
	BOOL bRet = ERR_SUCCESS;  				//返回结果
	char szHeadBmp[ 260 ] = { 0 };         //用于保存临时身份证图片的目录

	//获取图片缓存目录
	sprintf( szHeadBmp,"%s%s",FILE_TMP_DIR,FILE_TMP_HEAD);
	
	//保存二代证头像
	if( szHeadDir != NULL && strlen(szHeadDir) > 0 )
	{
		bRet = SavePhotoImage( szData, szHeadBmp);
		if( bRet == ERR_SUCCESS )
		{
			bRet = CT_SaveHeadImg( cardId, szHeadDir);
		}
	}
	
	return bRet;
}

