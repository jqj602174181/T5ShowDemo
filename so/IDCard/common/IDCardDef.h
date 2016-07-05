////////////////////////////////////////////////////////////////////////////
//���ߣ�  guocuiping
//�ļ�����IDCardDef.h
//������  ���巵��ֵ
//���ڣ�  2013.03.05
////////////////////////////////////////////////////////////////////////////
#ifndef   _IDCARD_DEF_
#define   _IDCARD_DEF_


//���ؽ��
#define     ERR_SUCCESS       0    //�ɹ�
#define     ERR_DEVICE       -1    //�豸����
#define     ERR_RECV         -2    //���մ���
#define     ERR_SEND         -3    //���ʹ���
#define     ERR_CHECK        -4    //У�����
#define     ERR_TIMEOUT      -5    //��ʱ
#define     ERR_READ         -6    //��������
#define     ERR_IMAGE        -7    //����ͷ�����
#define     ERR_SAVEIMG      -8    //����ͼ�����
#define     ERR_CANCEL       -9    //ȡ������
#define     ERR_OTHER        -100  //��������


//��������Ҫ��ȡ�����֤ͼƬ���Ͷ���
#define		IMG_HEAD			0
#define		IMG_FRONT			1
#define		IMG_BACK			2
#define		IMG_IDCARD          3

//������Ϣ
typedef struct PERSONINFO
{
	char name[32];
	char sex[4];
	char nation[20];
	char birthday[12];
	char address[72];
	char cardId[20];
	char police[32];
	char validStart[12];
	char validEnd[12];
	char sexCode[4];
	char nationCode[4];
	char appendMsg[72];

}PERSONINFO;


#endif