#ifndef   _HEADER_FILE_IDCARD_
#define   _HEADER_FILE_IDCARD_

#include "GlobalDef.h"
#include "IDCardDef.h"

#ifdef __cplusplus
extern "C" {
#endif


/************************************************************************************
* 函数名：ReadIDCardInfo
*  
* 功能：  读二代证信息，保存到缓冲区
*  
*参数：   pszDevice[in]-----通讯设备,Window与Linux通用的为SDT,HID,PCSC，Linux下支持AUX1...，
									另外Window下串口格式刷为COM1, Linux下串口输入视系统而定
*         BpNo[in]------扩展口，NULL或""—无扩展口，其他—扩展口
*         nBaud[in]-----波特率
*         nTermType[in]-----终端类型,此参数只在Linux下有效
*         nTimeOut[in]--超时，单位：ms
*         pData[out]--二代证数据
*  
*返回值： >=0---pData数据长度
*         ERR_DEVICE---设备错误
*         ERR_SEND-----发送错误
*         ERR_RECV-----接收错误
*         ERR_TIMEOUT--超时
*         ERR_CHECK----校验错误
*         ERR_NOCARD---无二代证
************************************************************************************/
BOOL ReadIDCardInfo( const char *pszDevice, const char* szBpNo, int nBaud, int nTermType,
					 int nTimeOut, BYTE* pData, int* npDataLen );



/************************************************************************************
* 函数名：CancelRead
*  
* 功能：  取消正在进行的读卡操作
*  
*返回值：无
************************************************************************************/
long CancelRead( );



/************************************************************************************
* 函数名：GetPhotoData
*  
* 功能：  读取二代证个人数据
*  
*参数：   pData[in]---------二代证数据
*         pInfo[out]--------二代证文字数据结构
*  
*返回值：无
************************************************************************************/
void GetPersonInfo( BYTE* pData, PERSONINFO* pInfo );


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
int GetPhotoData( BYTE* pData, BYTE* pPhoto );



/****************************************************************************
*  函数名： GetNation
*
*  功能：   将民族代码转为民族内容
*
*  参数：   nIndex[ out ]--民族代码
*           szNation[ in ]--存放转换后民族内容
*			nBuffLen[ in ]--szNation长度
*  返回值： 无
*****************************************************************************/
void GetNation( int nIndex, char *szNation , int nBuffLen);




/****************************************************************************
*  函数名： GetPersonParam
*
*  功能：   获取身份证参数，将pSrc内容转为ASIC码
*
*  参数：   pSrc[ in ]--需要转换的内容
*           szDes[ out ]--存放转换后内容
*           nLen[ in ]--pSrc要转的长度
*			bFullCopy[ in ]--是否完全拷贝字符串信息
*  返回值： pSrc转换的长度
*****************************************************************************/
int GetPersonParam( BYTE* pSrc, char* szDes, int nLen , BOOL bFullCopy);



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
long SavePhotoImage( BYTE* pData, char* szBmpPath );


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/


#endif/*!<_HEADER_FILE_IDCARD_*/