#include <fcntl.h>
#include <errno.h>
#include <jni.h>
#include <debugLog.h>
#include <stdio.h>
#include <string.h>
#include "IccReader.h"
#include "queue.h"
#include "read_fifo.h"
#include "strOp.h"  /*Hex2AscII*/





/*!
*\brief  写buffer
*\jdata  数据
*\len    数据长度
*\return 0成功，小于0失败
*/

static jint write_buffer_jni( JNIEnv* env, jobject obj, jbyteArray jdata, jint len )
{
    int retval = 0;
    unsigned char* data = ( unsigned char *)(*env)->GetByteArrayElements( env, jdata, NULL );  

    retval = write_buffer( data, len );

    (*env)->ReleaseByteArrayElements( env, jdata, ( char *)data, 0 );

    return retval;
}

/*!
*\brief  读buffer
*\timeout  时延
*\len      接收数据长度
*\jbuf     接收buff
*\return 0成功，小于0失败
*/

static jint read_buffer_jni( JNIEnv* env, jobject obj, jint timeout, jint len, jbyteArray jbuf )
{

    int retval = 0;

    unsigned char* buf = ( unsigned char *)(*env)->GetByteArrayElements( env, jbuf, NULL );  

    retval = read_buffer( buf, len, timeout );

    (*env)->ReleaseByteArrayElements( env, jbuf, ( char *)buf, 0 );

    return retval;
}

/*!
*\brief  获取IC卡卡号
*\timeout  时延
*\nicFlag为卡类型：1接触；2非接； 3自动判断
*\szICCardNo     接收buff
*\return 0成功，小于0失败
*/


//int CT_GetIccInfo(int iIcFlag, char* pszAidList, char *pszTaglist, int* nInfoLen, char *pszUserInfo, int ntimeout) ;
static jint CT_GetIccInfo_jni(JNIEnv* env, jobject thiz, jint iIcFlag, jstring jstrAidList, jstring jstrTaglist, jbyteArray jstrUserInfo, jint ntimeout)
{
    jint retval = -1 ;
    jint piInfoLen = 0 ; 
    const char *pszAidList = (*env)->GetStringUTFChars(env, jstrAidList, 0) ;
    const char *pszTaglist = (*env)->GetStringUTFChars(env, jstrTaglist, 0) ;

   // LOGI("[CT_GetIccInfo_jni]iIcFlag=%d, jstrAidList=%s, jstrTaglist=%s, ntimeout=%d", iIcFlag, pszAidList,pszTaglist,ntimeout) ;

    char* pszUserInfo = ( char*)(*env)->GetByteArrayElements( env, jstrUserInfo, NULL );

    retval = CT_GetIccInfo(iIcFlag, pszAidList, pszTaglist, &piInfoLen, pszUserInfo, ntimeout) ;
    
   // LOGI("[CT_GetIccInfo_jni]  pszUserInfo=%s", pszUserInfo) ;
    
    (*env)->ReleaseStringUTFChars(env, jstrAidList, pszAidList) ;
    (*env)->ReleaseStringUTFChars(env, jstrTaglist, pszTaglist) ;
    (*env)->ReleaseByteArrayElements( env, jstrUserInfo,  (char *)pszUserInfo, 0 );
    return retval ;
}
//int CT_GenerateARQC(int iIcFlag, char pszAidList, char *pszTxData, int* nArqcLen, char *pszARQC, int ntimeout)
static jint CT_GenerateARQC_jni(JNIEnv* env, jobject thiz, jint iIcFlag, jstring jstrAidList,jstring jstrTxData,  jbyteArray jstrARQC, jint ntimeout )
{
    jint retval = -1 ;
    jint piArqcLen = 0 ;
    const char *pszAidList = (*env)->GetStringUTFChars(env, jstrAidList, 0);
    const char *pszTxData = (*env)->GetStringUTFChars(env, jstrTxData, 0);
    char* pszARQC = ( char*)(*env)->GetByteArrayElements( env, jstrARQC, NULL );

	
   // LOGI("[CT_GenerateARQC_jni]iIcFlag=%d, jstrAidList=%s, jstrTxData=%s, ntimeout=%d", iIcFlag, pszAidList,pszTxData,ntimeout) ;
    retval =CT_GenerateARQC(iIcFlag, pszAidList, pszTxData, &piArqcLen, pszARQC, ntimeout) ;
   //LOGI("[CT_GetIccInfo_jni]  pszARQC=%s", pszARQC) ;
	
    (*env)->ReleaseStringUTFChars(env, jstrAidList, pszAidList);
    (*env)->ReleaseStringUTFChars(env, jstrTxData, pszTxData);
    (*env)->ReleaseByteArrayElements( env, jstrARQC,  (char *)pszARQC, 0 );

    return retval ;
}

//int CT_ExeICScript(int iIcFlag, char *pszTxData, char *pszARPC,int iStatus,char*pszARQC ,int ntimeout ，char *pszScriptResult, int* nTClen, char *pszTC )
static jint CT_ExeICScript_jni(JNIEnv* env, jobject thiz, jint iIcFlag, jstring jstrTxData, jstring jstrARPC, jint iStatus, jbyteArray jstrSptResult,  jbyteArray jstrTc, jint ntimeout)
{
    jint retval = -1 ;
    jint piTClen = 0 ;
    const char *pszTxData = (*env)->GetStringUTFChars(env, jstrTxData, 0);
    const char *pszARPC = (*env)->GetStringUTFChars(env, jstrARPC, 0);
   // const char *pszARQC = (*env)->GetStringUTFChars(env, jstrARQC, 0);
    char* pszScriptResult = ( char*)(*env)->GetByteArrayElements( env, jstrSptResult, NULL );
    char* pszTC = ( char*)(*env)->GetByteArrayElements( env, jstrSptResult, NULL );

    retval =CT_ExeICScript(iIcFlag, pszTxData, pszARPC, iStatus, NULL, ntimeout, pszScriptResult, &piTClen, pszTC) ;
   
	  
    (*env)->ReleaseStringUTFChars(env, jstrTxData, pszTxData);
    (*env)->ReleaseStringUTFChars(env, jstrARPC, pszARPC);
   // (*env)->ReleaseStringUTFChars(env, jstrARQC, pszARQC);
    (*env)->ReleaseByteArrayElements( env, jstrSptResult,  (char *)pszScriptResult, 0 );
    (*env)->ReleaseByteArrayElements( env, jstrTc,  (char *)pszTC, 0 );

    return retval ;
}

//int CT_GetTxDetail(int iIcFlag, char*pszAidList, int ntimeout, int* nTxDetailLen, char*pszTxDetail) ;
static jint CT_GetTxDetail_jni(JNIEnv* env, jobject thiz, jint iIcFlag, jstring jstrAidList, jbyteArray jstrTxDetail, jint ntimeout )
{
    jint retval = -1 ;
    jint piTxDetailLen = 0 ;
    const char* pszAidList = (*env)->GetStringUTFChars(env, jstrAidList, 0);
    char* pszTxDetail = ( char*)(*env)->GetByteArrayElements( env, jstrTxDetail, NULL );

    retval =CT_GetTxDetail(iIcFlag, pszAidList, ntimeout, &piTxDetailLen, pszTxDetail) ;

    (*env)->ReleaseStringUTFChars(env, jstrAidList, pszAidList);
    (*env)->ReleaseByteArrayElements( env, jstrTxDetail,  (char *)pszTxDetail, 0 );
    
    return retval ;
}

/* Native 方法 */
static JNINativeMethod NativeMethodTable[] = 
{
    { "write_buffer", "([BI)I", (void *)write_buffer_jni },
    { "read_buffer", "(II[B)I", (void *)read_buffer_jni },
    { "CT_GetIccInfo",  "(ILjava/lang/String;Ljava/lang/String;[BI)I",      (void *)CT_GetIccInfo_jni   },
    { "CT_GenerateARQC","(ILjava/lang/String;Ljava/lang/String;[BI)I",      (void*)CT_GenerateARQC_jni  },
    { "CT_ExeICScript", "(ILjava/lang/String;Ljava/lang/String;I[B[BI)I",   (void*)CT_ExeICScript_jni   },
    { "CT_GetTxDetail", "(ILjava/lang/String;[BI)I",                        (void*)CT_GetTxDetail_jni   }

} ;

/*!
 * \brief 注册JNI的方法到对应的类中
 * \param env - java的接口指针，java interface pointer
 * \return JNI_OK(即0)表示成功，JNI_ERR(即-1)表示失败
 */
static int registerMethods( JNIEnv* env ) 
{
    static const char* const kClassName ="com/centerm/iccard/IcCard";
    jclass clazz;

    clazz =(*env)->FindClass( env, kClassName );/*查看类定义*/    
    
    LOGE(" find (%s) class now\n", kClassName);    
    if( NULL == clazz )
    {
        LOGE("Can't find class %s\n", kClassName);    
        return JNI_ERR;
    }
    
    /*注册方法*/
    if((*env)->RegisterNatives( env, clazz, NativeMethodTable, sizeof(NativeMethodTable)/sizeof(JNINativeMethod)) != JNI_OK )
    {
        LOGE( "Failed registering methods for %s\n", kClassName);
        return JNI_ERR;
    }

    return JNI_OK;/*成功返回*/
}

/*!
 * \brief 注册JNI的方法到对应的类中
 * \param env - java的接口指针，java interface pointer
 * \return 0表示成功，-1表示失败
 */
jint JNI_OnLoad( JavaVM* vm, void* reserved ) 
{ 
    JNIEnv* env = NULL;    
    jint result = JNI_ERR;//返回值
    
    if( (*vm)->GetEnv( vm, (void**) &env, JNI_VERSION_1_4 ) != JNI_OK ) 
    {
        goto fail;
    }

    if( registerMethods(env) != 0)
    { 
        //注册你的 JNINativeMethod
        goto fail;
    }

    //通讯环境初始化;
    fifo_init();

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;
fail:

    return result;
}



#if defined(FUNCTION_TEST) 

int main(int argc, char const *argv[])
{
    int retval = 0;
	/*
    char szCardNo[256] = {0};

    printf("Get ICCardNo. \n") ;

    int nComNo = 2 ;
    int nTimeOut = 20 ;
    int iIcFlag = 1 ;

    fifo_init();

    retval = getICCardNo_jni(nComNo, &iIcFlag, nTimeOut, szCardNo) ;
    
	if(0 == retval)
    {
        printf("Get ICCardNo Success szCardNo:%s IcFlag=%d\n", szCardNo, iIcFlag) ;
    }
    else
    {
        printf("Get ICCardNo failed   %d\n", retval) ;
    }
	*/
	
	int writestate = -1 , readstate = -1 ;
	unsigned char writeBuf[] = { 0xAA,0xBB,0xCC,0xDD } ;
	unsigned char readBuf[20] = { 0} ;
	printf("call  fifo_write \n" ) ;
	writestate = fifo_write( writeBuf , 4 ) ;
	
	printf("call read_buffer \n") ;
	readstate = read_buffer( readBuf, 20, 10000 ) ;
	
	char temp[20] = {0} ;
	HexToAscii(readBuf, readstate, temp) ;
	printf("readbuf =%s %d \n", temp, readstate ) ;
	
	
	unsigned char writeBuf1[] = { 0x11,0x22,0x33,0x03 } ;
	unsigned char readBuf1[20] = { 0} ;
	printf("call  write_buffer \n" ) ;
	writestate = write_buffer( writeBuf1 , 4 ) ;
	
	readstate = 0 ;
	
	printf("call fifo_read \n") ;
	readstate = fifo_read( readBuf1, 20, 10 ) ;
	printf("fifo_read  retval = %d \n ", readstate) ;
	
	HexToAscii(readBuf1, readstate, temp) ;
	
	printf("fifo_read  readbuf = %s \n", temp) ;
	
    return retval;
}

#endif










