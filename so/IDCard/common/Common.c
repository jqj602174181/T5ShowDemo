#include <stdio.h>
#include "Common.h"
#include "Base64.h"
#include "GlobalDef.h"
#include "Debug.h"
#include "debugLog.h"
#if defined( WINDOWS_EDITION )
#include <TCHAR.h>
#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib")
#else
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include <sys/time.h> 
#include <sys/types.h>
#include <dirent.h>
#include "gbk2ucs.h"
#include <strings.h>
//#include <cstring> //C++
#define  strnicmp  strncasecmp
#define  stricmp   strcasecmp
#endif



/****************************************************************************
**************************Linux平台下的函数**********************************
/****************************************************************************/


#if defined(LINUX_EDITION )  

// 返回自系统开机以来的毫秒数（tick）  
unsigned long GetTickCount()  
{  
	struct timeval tv;
	if( gettimeofday(&tv, NULL) != 0 )
		return 0;
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}  

//Linux下的多字节转UTF8
int UnicodeToUTF8(WORD *pUnicodeBuf, int nUnicodeBufLen, BYTE *pUtfBuf, int nUtfBufSize)
{
	BYTE *p = pUtfBuf;
	WORD* pUnicode = pUnicodeBuf;
	WORD wch = 0;
	int nwchLen = 0;
	int nConvertLen = 0;

	if ( (pUnicodeBuf == NULL) || (pUtfBuf == NULL) )
	{
		return nConvertLen;
	}

	for(nwchLen=0; (nwchLen < nUnicodeBufLen) && ((p - pUtfBuf) < (nUtfBufSize -1)); nwchLen+=sizeof(WORD),pUnicode++ )
	{
		wch = *pUnicode;
		if(wch < 0x0080)
		{
			*p++ = (BYTE)wch;
		}
		else if(wch < 0x0800)
		{
			*p++ = (0xC0 | (BYTE)(wch >> 6));
			*p++ = (0x80 | (BYTE)(wch & 0x3F));
		}
		else
		{
			*p++ = (0xE0 | (BYTE)(wch >> 12));
			*p++ = (0x80 | (BYTE)((wch >> 6) & 0x3F));
			*p++ = (0x80 | (BYTE)(wch & 0x3F));
		}
		++nConvertLen;
	}

	*p = '\0';

	return nConvertLen;
}

//Linux下的GBK转UTF8
void GBKToUTF8( const char* szSrc, BYTE *pUtfBuf, int nUtfBufSize )
{
	unsigned short* wcstr = (unsigned short*)malloc( (strlen(szSrc)+1)*2 );
	int nwcstrlen = 0;

	memset( wcstr, 0, (strlen(szSrc)+1)*2 );
	nwcstrlen = GbkToUcs2( wcstr, szSrc, strlen(szSrc) );
	UnicodeToUTF8(wcstr, nwcstrlen*sizeof(unsigned short), pUtfBuf, nUtfBufSize);
	
	free( wcstr );
}

//判断路径是否指向一个目录
int PathIsDirectory(const char* szDir)
{
	int nRet = 0;
	if(szDir != NULL && strlen(szDir) > 0)
	{
		//如果是以‘/’结尾则说明是目录文件
		if(szDir[strlen(szDir) - 1] == '/'
			|| NULL!=opendir(szDir) )
		{
			 nRet = 1;
		}
	}	
	
	return nRet;
}



/**
 * 功能：拷贝文件函数
 * 参数：
 * 		srcFile：源文件名（带路径）
 * 		targetFile：目标文件名（带路径）
 *      nOverWrite: 1—如果目标存在文件则重写， 0-不重写
 * 返回值：
 * 		1: 拷贝成功
 * 		0：拷贝失败
 * author:wangchangshuai jlu
 */
int CopyFile(const char *srcFile, const char *targetFile, int nOverWrite)
{
	FILE *fpR  = NULL;
	FILE *fpW  = NULL;
	char buffer[1024];
	int lenR = 0, lenW = 0;
	int nRet = 0;
	char szOpenType[10] = {0};
	
	//判断是否重写
	if(nOverWrite)
	{
		strcpy(szOpenType,"wb");
	}
	else
	{
		if( (fpW = fopen(targetFile, "r")) != NULL )
		{
			fclose(fpW);
			return nRet;
		}	
	}
	
	if ( (fpR = fopen(srcFile, "r")) != NULL
           && (fpW = fopen(targetFile, "wb")) != NULL )
	{
		nRet = 1;
		memset(buffer, 0, sizeof(buffer));
		while ((lenR = fread(buffer, 1, sizeof(buffer), fpR)) > 0)
		{
			if ((lenW = fwrite(buffer, 1, lenR, fpW)) != lenR)
			{
				nRet = 0;
				break;
			}
			memset(buffer, 0, sizeof(buffer));
		}
	}

	//释放文件描述符
	fclose(fpR);
	fclose(fpW);
	
	return nRet;
}


#endif //LINUX_EDITION





/****************************************************************************
**************************Windows平台下的函数********************************
/****************************************************************************/


#if defined(WINDOWS_EDITION )  

//宽字节转多字节--Windows版本
int UnicodeToAnsi(const wchar_t* pszSrc, int nSrcLen, char *pszDest, int nDestSize)
{
	int nRetLen = 0;
	int nNeedLen = 0;
	char* result = NULL;

	//参数判断
	if (pszSrc != NULL)
	{
		//计算所需个数
		nNeedLen = WideCharToMultiByte( CP_ACP, 0, pszSrc, -1, NULL, 0, NULL, NULL );
		if (nNeedLen+1 < nDestSize)
		{
			//转换
			result = (char *)calloc(sizeof(char), nNeedLen+1);
			if (result)
			{
				nRetLen = WideCharToMultiByte( CP_ACP, 0, pszSrc, -1, result, nNeedLen+1, NULL, NULL );
				memcpy(pszDest, result, nRetLen);
				free(result);
			}
		}
	}
	return nRetLen;
}

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
void GetFullPath( char* szFileName, char* szPath, int nPathLen )
{
	char szTmpPath[ MAX_PATH ] = { 0 };
	char* p = NULL;
	MEMORY_BASIC_INFORMATION info;
	VirtualQuery(GetFullPath, &info, sizeof(info));

	GetModuleFileNameA( (HMODULE)(info.AllocationBase), szTmpPath, sizeof(szTmpPath) );

	p = strrchr( szTmpPath, '\\' );
	*( p+1 ) = 0;

	_stprintf_s( szPath, nPathLen, "%s%s", szTmpPath, szFileName );
}


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
void GetTmpFile( char* szName, char* szPath, int nPathLen )
{
	char szTmpPath[ MAX_PATH ] = { 0 };

	GetTempPathA( sizeof(szTmpPath)-1, szTmpPath );
	_snprintf( szPath, nPathLen, "%s%s", szTmpPath, szName );
}



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
BOOL WriteFileData( const char* szImgFile, BYTE* pData, int nLen )
{
	BOOL bRet = FALSE;
	FILE* fp = NULL;

	fp = fopen( szImgFile, "wb" );
	if( fp != NULL )
	{
		if( fwrite(pData, 1, nLen, fp) >= nLen )
		{
			 bRet = TRUE;
		}
	}

	if( fp != NULL )
	{
		fclose( fp );
	}


	return bRet;
}


/************************************************************************************
* 函数名：ReadFileData
*  
* 功能：  读文件
*  
*参数：   szImgFile[in]--文件
*         pData[out]------读出文件数据
*         nLen[out]-------实际读出长度
*  
*返回值： TRUE---成功
*         FALSE--失败
************************************************************************************/
int ReadFileData( const char* szImgFile, BYTE* pData, int* npLen )
{
	DWORD nCount = 0;
	DWORD nRead = 0;
	DWORD nDataLen = 0;
	HANDLE hFile = 0;
	int nRet = 0;

	hFile = CreateFileA( szImgFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return -1;
	}

	nDataLen = GetFileSize( hFile, NULL );
	if( nDataLen > *npLen || pData == NULL )
	{
		*npLen = nDataLen;
		CloseHandle( hFile );
		return -2;
	}

	while( nCount < nDataLen )
	{
		nRet = ReadFile( hFile, pData+nCount, nDataLen-nCount, &nRead, NULL );
		if( nRet != 0 )
		{
			if( nRead == 0 )
			{
				break;
			}
			else
			{
				nCount += nRead;
			}
		}
		else
		{
			CloseHandle( hFile );
			return -1;
		}
	}

	CloseHandle( hFile );
	*npLen = nCount;
	return 0;
}


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
BOOL GetBase46FromFile( const char* szFile, char* szBase, int* npBase )
{
	BYTE* pData = NULL;
	int nDataLen = 0;
	int nBaseLen = 0;
	int nRet = -2;

	 if( ReadFileData( szFile, NULL, &nDataLen ) != -2 )
	 {
		 return nRet;
	 }

	 nBaseLen = (nDataLen+2)/3*4;
	 if( szBase == NULL || *npBase < nBaseLen )
	 {
		 *npBase = nBaseLen;
		 return -2;
	 }

	 pData = (BYTE*)calloc( nDataLen, sizeof(BYTE));
	 if( ReadFileData( szFile, pData, &nDataLen ) != 0 )
	 {
		 goto End;
	 }

	 memset( szBase, 0, *npBase );
	 nBaseLen = EncryptBase64( pData, nDataLen, (BYTE*)szBase, *npBase );
	 if( nBaseLen > 0 )
	 {
		 *npBase = nBaseLen;
		 nRet = 0;
	 }

End:
	 free(pData);
	 return nRet;
}

#endif  //WINDOWS_EDITION


/****************************************************************************
******************************通用函数****************************************
/****************************************************************************/


/****************************************************************************
*  函数名： GeneralSleep
*
*  功能：   Window与Linux通用的Sleep函数，单位为毫秒
*
*  参数：   nMicroSeconds[ in ]--毫秒数
*  返回值： 无
*****************************************************************************/
void GeneralSleep(int nMicroSeconds )
{
	#if defined( WINDOWS_EDITION )
		Sleep(nMicroSeconds);
	#else
		usleep(nMicroSeconds * 1000);
	#endif
}


/**********************************************************
*  函数名： Check
*
*  功能：   异或和校验是否正确，结果0为正确，其他失败
*
*  参数：   pszBuf[ in ]--校验数据
*           dwLen[ in ]-- 校验字节
*
*  返回值： 1--成功
*           0--失败
**********************************************************/
BOOL Check( BYTE* pszBuf, int nLen )
{
	int i =0;
	BYTE bTemp = pszBuf[ 0 ];

	//异或和
	for( i = 1; i < nLen; i++ )
	{
		bTemp ^= pszBuf[ i ];
	}
	if( bTemp )  
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


/****************************************************************************
*  函数名： GetImgPathEx
*
*  功能：   如果传入的是目录则使用默认的文件名保存，否则则按传进来的路径保存
*
*  参数：   szImgDir[ in ]--图片保存路径
*           szImgType[ in ]--指定类型的字符串
*			pDefFileName[ in ]--默认文件名
*           szPath[ out ]--保存输出图片路径的字符串
*			nPathLen[ in ]--图片路径的字符串的最大长度
*  返回值： 无
*****************************************************************************/
void GetImgPathEx(const char* szImgDir,  const char* szImgType, 
				const char* pDefFileName,  char* szPath, const int nPathLen )
{
	char szTmpImgType[ 260 ] = { 0 };
	char szTmpDir[ 260 ] = { 0 };
	char szName[ 260 ] = { 0 };
	const char* pTmp = NULL;
	
	//Window和Linux下的路径描述符
#ifdef WINDOWS_EDITION
	char chSpilt = '\\';
#else
	char chSpilt = '/';
#endif
	
	if( szImgDir == NULL )
	{
		return;
	}

	if( szImgType != NULL && strlen(szImgType)> 0 )
	{
		sprintf( szTmpImgType, "%s", szImgType );
	}

	if( PathIsDirectory(szImgDir) )//szDir是目录
	{
		if( szImgDir[ strlen(szImgDir)-1] != chSpilt) //最后一个字符非分隔符
		{
			if(strlen(szImgType)> 0)
			{
				sprintf( szPath,  "%s%c%s%s", szImgDir, chSpilt, pDefFileName, szTmpImgType );	
			}
			else
			{
				sprintf( szPath,  "%s%c%s", szImgDir, chSpilt, pDefFileName);	
			}		
		}
		else//最后一个字符是分隔符
		{

			if(strlen(szImgType)> 0)
			{
				sprintf( szPath,  "%s%s%s", szImgDir, pDefFileName, szTmpImgType );	
			}
			else
			{
				sprintf( szPath,  "%s%s", szImgDir, pDefFileName );	
			}	
			LOGI("GetImgPathEx: szPath = %s\n",szPath);
		}
	}
	else //szDir是文件
	{
		pTmp = strrchr(szImgDir, chSpilt );

		if( pTmp != NULL )
		{
			pTmp += 1;
			strncpy(szTmpDir, szImgDir, pTmp-szImgDir);
		}
		else
		{
			pTmp = szImgDir;
		}

		if( stricmp(pTmp, "*.bmp") == 0 ||
			stricmp(pTmp, "*.jpg") == 0 ||
			stricmp(pTmp, "*.png") == 0 ||
			stricmp(pTmp, "*.gif") == 0  )
		{
			strcpy( szName, pTmp+1 );
			sprintf( szPath, "%s%s%s%s", szTmpDir, pDefFileName, szTmpImgType, szName );
		}
		else
		{
			strncpy( szPath, szImgDir, nPathLen );
		}
	}
	LOGI("GetImgPathEx: szPath = %s\n",szPath);
}


