#include <string.h>
#include "IDCard.h"
#include "Common.h"
#include "IDCardDef.h"
#include "GlobalDef.h"
#include "Debug.h"
#include <stdio.h>

#ifdef WINDOWS_EDITION
	#include "WindowImage.h"
	#include <string.h>
#else
	#include "LinuxImage.h"
	#include <strings.h>
	#define  strnicmp  strncasecmp
	#define  stricmp   strcasecmp
#endif


/************************************************************************************
* 函数名：GetPhotoData
*  
* 功能：  读取二代证头像数据
*  
*参数：   pData[in]---------二代证数据
*         pPhoto[out]-------二代证头像数据
*  
*返回值： 二代证头像数据长度
************************************************************************************/
int GetPhotoData( BYTE* pData, BYTE* pPhoto )
{
	int nHeader = 10;
	int nTextLenLen = 2;
	int nPhotoLenLen = 2;
	int nTextLen = 0;
	int nPhotoLen = 0;
	int nCur = 0;

	nTextLen = pData[ nHeader ] * 256 + pData[ nHeader+1 ];
	nPhotoLen = pData[ nHeader+nTextLenLen ] * 256 + pData[ nHeader+nTextLenLen+1 ];
	nCur = nHeader + nTextLenLen + nPhotoLenLen + nTextLen;

	memcpy( pPhoto, pData+nCur, nPhotoLen );
	return nPhotoLen;
}

/************************************************************************************
* 函数名：SavePhotoImage
*  
* 功能：  取得二代证头像
*  
*参数：   pData[in]--------头像数据
*         szBmpPath[in]--头像保存路径，bmp格式
*  
*返回值： ERROR_SUCCESS--成功
*         ERR_IMAGE------失败
************************************************************************************/
long SavePhotoImage( BYTE* pData, char* szBmpPath )
{
	BYTE szPhoto[ 2048 ] = { 0 };  //头像数据
	int nPhotoLen = 0;             //头像数据长度
	char szwltFile[ 260 ] = { 0 };  //wlt文件路径
	char szDllPath[ 260 ] = { 0 };      //WltRS.dll路径
	char* pTemp = NULL;  
	int nRet = ERR_SUCCESS; 

	//取得头像数据
	nPhotoLen = GetPhotoData( pData, szPhoto );

#ifdef WINDOWS_EDITION
	//将szPhotoPath文件改为wlt格式
	strncpy( szwltFile, szBmpPath, sizeof(szwltFile)-1 );
	pTemp = strrchr( szwltFile, '.' );
	*(pTemp+1) = 0;
	strcat( szwltFile, "wlt" );

	//获取Dll的路径
	GetFullPath( FILE_WLTRS, szDllPath, _countof(szDllPath) );

	//将数据写到szwltFile，并转为bmp图片
	if( !WriteFileData(szwltFile, szPhoto, nPhotoLen)||
		!GetPhotoBmp(szDllPath, szwltFile) )
	{
		nRet = ERR_IMAGE;
	}

#else 
	if( !GetPhotoBmp(FILE_WLTRS, (char *)szPhoto, szBmpPath) )
	{
		nRet = ERR_IMAGE;
	}
#endif

	return nRet;
}
