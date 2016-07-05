#include <fcntl.h>
#include <errno.h>
#include <jni.h>
#include "debugLog.h"
#include "CENT_IDCard.h"
#include "PrjDef.h"





/*!\brief JNI_SaveHeadImg
* \par 说明：
* \ param[in] szHeadDir--头像保存目录，取值为NULL或“”时取消保存图片
* \ param[in] pInfo----二代证文字信息
* \ param[in] cardId ----二代证身份证号
* return 0---成功，否则失败
*/
static jint JNI_SaveHeadImg( JNIEnv* env, jobject thiz,jbyteArray szHeadDir, jbyteArray pInfo ,jbyteArray cardId )
{
    jint retval = -1;
	

    unsigned char* PszHeadDir = NULL;
    unsigned char* szpInfo = NULL;
    unsigned char* szcardId = NULL;    
	
    if(NULL != szHeadDir)
    {
        PszHeadDir = ( unsigned char *)(*env)->GetByteArrayElements( env, szHeadDir, NULL );
    }
    if(NULL != pInfo)
    {
        szpInfo = ( unsigned char *)(*env)->GetByteArrayElements( env, pInfo, NULL );
    }
	if( NULL != szcardId)
	{
		szcardId = ( unsigned char *)(*env)->GetByteArrayElements( env, cardId, NULL );
	}

	retval = CT_GetHeadImg(PszHeadDir, szpInfo, szcardId );
     
    if(NULL != szHeadDir)
    {
        (*env)->ReleaseByteArrayElements( env, szHeadDir, ( char *)PszHeadDir, 0 );
    }

    if(NULL != pInfo)
    {
        (*env)->ReleaseByteArrayElements( env, pInfo, ( char *)szpInfo, 0 );
    }
	
	if( NULL != szcardId)
	{
		(*env)->ReleaseByteArrayElements( env, cardId, ( char *)szcardId, 0 );
	}

    return  retval;
}


/* Native 方法 */
static JNINativeMethod NativeMethodTable[] = 
{   
  
    { "SaveHeadImg", "([B[B[B)I", (void *)JNI_SaveHeadImg } ,
  

};


static int registerMethods( JNIEnv* env ) 
{
    static const char* const kClassName ="com/centerm/idcard/Idcard";
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
        LOGE("ERROR: GetEnv failed\n");
        goto fail;
    }

    if( registerMethods(env) != 0)
    { 
        //注册你的 JNINativeMethod
        LOGE("ERROR: PlatformLibrary native registration failed\n");
        goto fail;
    }


    /* success -- return valid version number */
    result = JNI_VERSION_1_4;
fail:

    return result;
}


