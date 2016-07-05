////////////////////////////////////////////////////////////////////////////
//作者：  guocuiping
//文件名：IDCardDef.h
//描述：  定义返回值
//日期：  2013.03.05
////////////////////////////////////////////////////////////////////////////
#ifndef   _IDCARD_DEF_
#define   _IDCARD_DEF_


//返回结果
#define     ERR_SUCCESS       0    //成功
#define     ERR_DEVICE       -1    //设备错误
#define     ERR_RECV         -2    //接收错误
#define     ERR_SEND         -3    //发送错误
#define     ERR_CHECK        -4    //校验错误
#define     ERR_TIMEOUT      -5    //超时
#define     ERR_READ         -6    //读卡错误
#define     ERR_IMAGE        -7    //解析头像错误
#define     ERR_SAVEIMG      -8    //保存图像错误
#define     ERR_CANCEL       -9    //取消操作
#define     ERR_OTHER        -100  //其他错误


//常量——要获取的身份证图片类型定义
#define		IMG_HEAD			0
#define		IMG_FRONT			1
#define		IMG_BACK			2
#define		IMG_IDCARD          3

//个人信息
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