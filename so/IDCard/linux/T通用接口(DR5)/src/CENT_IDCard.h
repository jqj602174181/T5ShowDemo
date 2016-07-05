#ifndef _CENT_IDCARD_H_
#define _CENT_IDCARD_H_

#include "GlobalDef.h"
#include "IDCardDef.h"

#ifdef __cplusplus
extern "C" {
#endif

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
long _STDOUT CT_GetHeadImg  ( char* szHeadDir, char* szData ,char* cardId);

#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif //_CENT_IDCARD_H_