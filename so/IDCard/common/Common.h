#ifndef   _HEADER_FILE_COMMON_
#define   _HEADER_FILE_COMMON_

#include "GlobalDef.h"
#include "IDCardDef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************
*  �������� Check
*
*  ���ܣ�   ����У���Ƿ���ȷ�����0Ϊ��ȷ������ʧ��
*
*  ������   pszBuf[ in ]--У������
*           dwLen[ in ]-- У���ֽ�
*
*  ����ֵ�� TRUE--1
*           FALSE--0
**********************************************************/
BOOL Check( BYTE* pszBuf, int nLen );




/****************************************************************************
*  �������� GeneralSleep
*
*  ���ܣ�   Window��Linuxͨ�õ�Sleep��������λΪ����
*
*  ������   nMicroSeconds[ in ]--������
*  ����ֵ�� ��
*****************************************************************************/
void GeneralSleep(int nMicroSeconds );



/****************************************************************************
*  �������� GetImgPathEx
*
*  ���ܣ�   ��ȡ�����֤��+ָ�����͵�ͼƬ����·��
*
*  ������   szImgDir[ in ]--ͼƬ����·��
*           szImgType[ in ]--ָ�����͵��ַ���
*			pDefFileName[ in ]--Ĭ���ļ���
*           szPath[ out ]--�������ͼƬ·�����ַ���
*			nPathLen[ in ]--ͼƬ·�����ַ�������󳤶�
*  ����ֵ�� ��
*****************************************************************************/
void GetImgPathEx(const char* szImgDir,  const char* szImgType, 
				const char* pDefFileName,  char* szPath, const int nPathLen );
				

/****************************************************************************
**************************Linuxƽ̨�µĺ���**********************************
/****************************************************************************/


#if defined(LINUX_EDITION )  

// ������ϵͳ���������ĺ�������tick��  
unsigned long GetTickCount();

//Linux�µĶ��ֽ�תUTF8
int UnicodeToUTF8(WORD *pUnicodeBuf, int nUnicodeBufLen, BYTE *pUtfBuf, int nUtfBufSize);

//Linux�µ�GBKתUTF8
void GBKToUTF8( const char* szSrc, unsigned char *pUtfBuf, int nUtfBufSize );

//Linux�µ�CopyFile
int CopyFile(const char *srcFile, const char *targetFile, int nOverWrite);

#endif




/****************************************************************************
**************************Windowsƽ̨�µĺ���********************************
/****************************************************************************/


#if defined(WINDOWS_EDITION )  


//���ֽ�ת���ֽ�--Windows�汾
int UnicodeToAnsi(const wchar_t* pszSrc, int nSrcLen, char *pszDest, int nDestSize);



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
void GetFullPath( char* szFileName, char* szPath, int nPathLen );


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
void GetTmpFile( char* szName, char* szPath, int nPathLen );


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
BOOL WriteFileData( const char* szImgFile, BYTE* pData, int nLen );


/************************************************************************************
* ��������ReadFileData
*  
* ���ܣ�  д�ļ�
*  
*������   szImgFile[in]--�ļ�
*         pData[out]------�����ļ�����
*         nLen[out]-------ʵ�ʶ�������
*  
*����ֵ�� TRUE---�ɹ�
*         FALSE--ʧ��
************************************************************************************/
int ReadFileData( const char* szImgFile, BYTE* pData, int* nLen );


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
BOOL GetBase46FromFile( const char* szFile, char* szBase, int* npBase );


#endif /*!<WINDOWS_EDITION*/


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif/*!< _HEADER_FILE_COMMON_*/