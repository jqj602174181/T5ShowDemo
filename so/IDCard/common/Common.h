#ifndef   _HEADER_FILE_COMMON_
#define   _HEADER_FILE_COMMON_

#include "GlobalDef.h"
#include "IDCardDef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************
*  函数名： Check
*
*  功能：   异或和校验是否正确，结果0为正确，其他失败
*
*  参数：   pszBuf[ in ]--校验数据
*           dwLen[ in ]-- 校验字节
*
*  返回值： TRUE--1
*           FALSE--0
**********************************************************/
BOOL Check( BYTE* pszBuf, int nLen );




/****************************************************************************
*  函数名： GeneralSleep
*
*  功能：   Window与Linux通用的Sleep函数，单位为毫秒
*
*  参数：   nMicroSeconds[ in ]--毫秒数
*  返回值： 无
*****************************************************************************/
void GeneralSleep(int nMicroSeconds );



/****************************************************************************
*  函数名： GetImgPathEx
*
*  功能：   获取以身份证号+指定类型的图片保存路径
*
*  参数：   szImgDir[ in ]--图片保存路径
*           szImgType[ in ]--指定类型的字符串
*			pDefFileName[ in ]--默认文件名
*           szPath[ out ]--保存输出图片路径的字符串
*			nPathLen[ in ]--图片路径的字符串的最大长度
*  返回值： 无
*****************************************************************************/
void GetImgPathEx(const char* szImgDir,  const char* szImgType, 
				const char* pDefFileName,  char* szPath, const int nPathLen );
				

/****************************************************************************
**************************Linux平台下的函数**********************************
/****************************************************************************/


#if defined(LINUX_EDITION )  

// 返回自系统开机以来的毫秒数（tick）  
unsigned long GetTickCount();

//Linux下的多字节转UTF8
int UnicodeToUTF8(WORD *pUnicodeBuf, int nUnicodeBufLen, BYTE *pUtfBuf, int nUtfBufSize);

//Linux下的GBK转UTF8
void GBKToUTF8( const char* szSrc, unsigned char *pUtfBuf, int nUtfBufSize );

//Linux下的CopyFile
int CopyFile(const char *srcFile, const char *targetFile, int nOverWrite);

#endif




/****************************************************************************
**************************Windows平台下的函数********************************
/****************************************************************************/


#if defined(WINDOWS_EDITION )  


//宽字节转多字节--Windows版本
int UnicodeToAnsi(const wchar_t* pszSrc, int nSrcLen, char *pszDest, int nDestSize);



/****************************************************************************
*  函数名： GetFullPath
*
*  功能：   拼接当前模块路径
*
*  参数：   szFileName[ in ]--要拼接的文件名
*           szPath[ out ]--保存输出目录的字符串
*			szPath[ in ]--输出目录的字符串的最大长度
*  返回值： 无
*****************************************************************************/
void GetFullPath( char* szFileName, char* szPath, int nPathLen );


/****************************************************************************
*  函数名： GetTmpFile
*
*  功能：   拼接缓存模块路径
*
*  参数：   szName[ in ]--要拼接的文件名
*           szPath[ out ]--保存输出目录的字符串
*			szPath[ in ]--输出目录的字符串的最大长度
*  返回值： 无
*****************************************************************************/
void GetTmpFile( char* szName, char* szPath, int nPathLen );


/************************************************************************************
* 函数名：WriteFileData
*  
* 功能：  写文件
*  
*参数：   szImgFile[in]--文件
*         pData[in]------写到文件数据
*         nLen[in]-------pData长度
*  
*返回值： TRUE---成功
*         FALSE--失败
************************************************************************************/
BOOL WriteFileData( const char* szImgFile, BYTE* pData, int nLen );


/************************************************************************************
* 函数名：ReadFileData
*  
* 功能：  写文件
*  
*参数：   szImgFile[in]--文件
*         pData[out]------读出文件数据
*         nLen[out]-------实际读出长度
*  
*返回值： TRUE---成功
*         FALSE--失败
************************************************************************************/
int ReadFileData( const char* szImgFile, BYTE* pData, int* nLen );


/****************************************************************************
*  函数名： GetBase46FromFile
*
*  功能：   获取身份证图片的base64
*
*  参数：   szFile[ in ]--文件名
*           szBase[ out ]--保存输出base64字符串
*			szPath[ int ]--输出base64字符串的长度
*  返回值： 无
*****************************************************************************/
BOOL GetBase46FromFile( const char* szFile, char* szBase, int* npBase );


#endif /*!<WINDOWS_EDITION*/


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif/*!< _HEADER_FILE_COMMON_*/