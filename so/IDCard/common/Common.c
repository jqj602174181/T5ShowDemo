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
**************************Linuxƽ̨�µĺ���**********************************
/****************************************************************************/


#if defined(LINUX_EDITION )  

// ������ϵͳ���������ĺ�������tick��  
unsigned long GetTickCount()  
{  
	struct timeval tv;
	if( gettimeofday(&tv, NULL) != 0 )
		return 0;
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}  

//Linux�µĶ��ֽ�תUTF8
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

//Linux�µ�GBKתUTF8
void GBKToUTF8( const char* szSrc, BYTE *pUtfBuf, int nUtfBufSize )
{
	unsigned short* wcstr = (unsigned short*)malloc( (strlen(szSrc)+1)*2 );
	int nwcstrlen = 0;

	memset( wcstr, 0, (strlen(szSrc)+1)*2 );
	nwcstrlen = GbkToUcs2( wcstr, szSrc, strlen(szSrc) );
	UnicodeToUTF8(wcstr, nwcstrlen*sizeof(unsigned short), pUtfBuf, nUtfBufSize);
	
	free( wcstr );
}

//�ж�·���Ƿ�ָ��һ��Ŀ¼
int PathIsDirectory(const char* szDir)
{
	int nRet = 0;
	if(szDir != NULL && strlen(szDir) > 0)
	{
		//������ԡ�/����β��˵����Ŀ¼�ļ�
		if(szDir[strlen(szDir) - 1] == '/'
			|| NULL!=opendir(szDir) )
		{
			 nRet = 1;
		}
	}	
	
	return nRet;
}



/**
 * ���ܣ������ļ�����
 * ������
 * 		srcFile��Դ�ļ�������·����
 * 		targetFile��Ŀ���ļ�������·����
 *      nOverWrite: 1�����Ŀ������ļ�����д�� 0-����д
 * ����ֵ��
 * 		1: �����ɹ�
 * 		0������ʧ��
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
	
	//�ж��Ƿ���д
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

	//�ͷ��ļ�������
	fclose(fpR);
	fclose(fpW);
	
	return nRet;
}


#endif //LINUX_EDITION





/****************************************************************************
**************************Windowsƽ̨�µĺ���********************************
/****************************************************************************/


#if defined(WINDOWS_EDITION )  

//���ֽ�ת���ֽ�--Windows�汾
int UnicodeToAnsi(const wchar_t* pszSrc, int nSrcLen, char *pszDest, int nDestSize)
{
	int nRetLen = 0;
	int nNeedLen = 0;
	char* result = NULL;

	//�����ж�
	if (pszSrc != NULL)
	{
		//�����������
		nNeedLen = WideCharToMultiByte( CP_ACP, 0, pszSrc, -1, NULL, 0, NULL, NULL );
		if (nNeedLen+1 < nDestSize)
		{
			//ת��
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
*  �������� GetFullPath
*
*  ���ܣ�   ƴ�ӵ�ǰģ��·��
*
*  ������   szFileName[ in ]--Ҫƴ�ӵ��ļ���
*           szPath[ out ]--�������Ŀ¼���ַ���
*			szPath[ in ]--���Ŀ¼���ַ�������󳤶�
*  ����ֵ�� ��
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
*  �������� GetTmpFile
*
*  ���ܣ�   ƴ�ӻ���ģ��·��
*
*  ������   szName[ in ]--Ҫƴ�ӵ��ļ���
*           szPath[ out ]--�������Ŀ¼���ַ���
*			szPath[ in ]--���Ŀ¼���ַ�������󳤶�
*  ����ֵ�� ��
*****************************************************************************/
void GetTmpFile( char* szName, char* szPath, int nPathLen )
{
	char szTmpPath[ MAX_PATH ] = { 0 };

	GetTempPathA( sizeof(szTmpPath)-1, szTmpPath );
	_snprintf( szPath, nPathLen, "%s%s", szTmpPath, szName );
}



/************************************************************************************
* ��������WriteFileData
*  
* ���ܣ�  д�ļ�
*  
*������   szImgFile[in]--�ļ�
*         pData[in]------д���ļ�����
*         nLen[in]-------pData����
*  
*����ֵ�� TRUE---�ɹ�
*         FALSE--ʧ��
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
* ��������ReadFileData
*  
* ���ܣ�  ���ļ�
*  
*������   szImgFile[in]--�ļ�
*         pData[out]------�����ļ�����
*         nLen[out]-------ʵ�ʶ�������
*  
*����ֵ�� TRUE---�ɹ�
*         FALSE--ʧ��
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
*  �������� GetBase46FromFile
*
*  ���ܣ�   ��ȡ���֤ͼƬ��base64
*
*  ������   szFile[ in ]--�ļ���
*           szBase[ out ]--�������base64�ַ���
*			szPath[ int ]--���base64�ַ����ĳ���
*  ����ֵ�� ��
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
******************************ͨ�ú���****************************************
/****************************************************************************/


/****************************************************************************
*  �������� GeneralSleep
*
*  ���ܣ�   Window��Linuxͨ�õ�Sleep��������λΪ����
*
*  ������   nMicroSeconds[ in ]--������
*  ����ֵ�� ��
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
*  �������� Check
*
*  ���ܣ�   ����У���Ƿ���ȷ�����0Ϊ��ȷ������ʧ��
*
*  ������   pszBuf[ in ]--У������
*           dwLen[ in ]-- У���ֽ�
*
*  ����ֵ�� 1--�ɹ�
*           0--ʧ��
**********************************************************/
BOOL Check( BYTE* pszBuf, int nLen )
{
	int i =0;
	BYTE bTemp = pszBuf[ 0 ];

	//����
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
*  �������� GetImgPathEx
*
*  ���ܣ�   ����������Ŀ¼��ʹ��Ĭ�ϵ��ļ������棬�����򰴴�������·������
*
*  ������   szImgDir[ in ]--ͼƬ����·��
*           szImgType[ in ]--ָ�����͵��ַ���
*			pDefFileName[ in ]--Ĭ���ļ���
*           szPath[ out ]--�������ͼƬ·�����ַ���
*			nPathLen[ in ]--ͼƬ·�����ַ�������󳤶�
*  ����ֵ�� ��
*****************************************************************************/
void GetImgPathEx(const char* szImgDir,  const char* szImgType, 
				const char* pDefFileName,  char* szPath, const int nPathLen )
{
	char szTmpImgType[ 260 ] = { 0 };
	char szTmpDir[ 260 ] = { 0 };
	char szName[ 260 ] = { 0 };
	const char* pTmp = NULL;
	
	//Window��Linux�µ�·��������
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

	if( PathIsDirectory(szImgDir) )//szDir��Ŀ¼
	{
		if( szImgDir[ strlen(szImgDir)-1] != chSpilt) //���һ���ַ��Ƿָ���
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
		else//���һ���ַ��Ƿָ���
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
	else //szDir���ļ�
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


