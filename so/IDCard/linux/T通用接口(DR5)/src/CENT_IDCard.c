// CENT_IDCard.c : ���� SO Ӧ�ó���ĵ���������
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

	//���û���·��
	sprintf( szHeadBmp,"%s%s",FILE_TMP_DIR,FILE_TMP_HEAD);
	
	if (szHeadImg != NULL)
	{
		//���szHeadImg�пռ䵫�޳��ȣ���ֵΪĬ�ϵ����֤ͼƬ·��
		if(strlen(szHeadImg) <= 0)
		{
			char *pTmp = strrchr(szHeadBmp, '/' ) + 1;
			strncpy(szHeadImg, szHeadBmp, pTmp - szHeadBmp);
		}
		pHeadImg = szHeadImg;
	}

	//���ñ���·��
	if(strlen(FILE_IMG_HEAD) > 0)
		GetImgPathEx( pHeadImg, "", FILE_IMG_HEAD, szImgPath, sizeof(szImgPath)-1 );
	else
		GetImgPathEx( pHeadImg, EXFILE_HEAD, cardId, szImgPath, sizeof(szImgPath)-1 );

	//����ͼƬ
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
* ��������CT_GetHeadImg
*  
* ���ܣ� ��ȡ����֤ͷ��
*  
*������ 
*         szHeadDir[in]--ͷ�񱣴�Ŀ¼��ȡֵΪNULL�򡰡�ʱȡ������ͼƬ
*         szData[in]----����֤ԭʼ��Ϣ
*		  cardId[in] ---����֤���֤��
*  
*����ֵ�� ERR_SUCCESS---�ɹ�
*         ERR_DEVICE---�豸����
*         ERR_SEND-----���ʹ���
*         ERR_RECV-----���մ���
*         ERR_CHECK-----У�����
*         ERR_TIMEOUT--��ʱ
*         ERR_READ-----��������
*         ERR_IMAGE-----����ͷ�����
*         ERR_SAVEIMG-----����ͼ�����
*		  ERR_CANCEL-----ȡ������
*         ERR_OTHER-----��������
************************************************************************************/
long _STDOUT CT_GetHeadImg   ( char* szHeadDir, char* szData ,char* cardId)
{
	
	BOOL bRet = ERR_SUCCESS;  				//���ؽ��
	char szHeadBmp[ 260 ] = { 0 };         //���ڱ�����ʱ���֤ͼƬ��Ŀ¼

	//��ȡͼƬ����Ŀ¼
	sprintf( szHeadBmp,"%s%s",FILE_TMP_DIR,FILE_TMP_HEAD);
	
	//�������֤ͷ��
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

