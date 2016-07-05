#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#include "PrjDef.h"

#if defined(DEBUG_OUTPUT)

#include<android/log.h>
#define LOG_TAG "debug_log"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#else

#define LOGI 
#define LOGD
#define LOGE

#endif

#endif
