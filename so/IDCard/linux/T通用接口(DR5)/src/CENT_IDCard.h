#ifndef _CENT_IDCARD_H_
#define _CENT_IDCARD_H_

#include "GlobalDef.h"
#include "IDCardDef.h"

#ifdef __cplusplus
extern "C" {
#endif

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
long _STDOUT CT_GetHeadImg  ( char* szHeadDir, char* szData ,char* cardId);

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif //_CENT_IDCARD_H_