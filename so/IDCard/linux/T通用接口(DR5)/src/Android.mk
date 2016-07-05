# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#



LOCAL_PATH := $(call my-dir)
#include $(CLEAR_VARS)
#LOCAL_MODULE := libidcaread
#LOCAL_SRC_FILES := libidcaread.so
#include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
MY_COMMON_DIR   := ../../../common
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_MODULE    := IDCard
#LOCAL_SHARED_LIBRARIES += libidcaread

LOCAL_SRC_FILES :=   IDCard_jni.c CENT_IDCard.c \
                    $(MY_COMMON_DIR)/IDCard.c  \
					$(MY_COMMON_DIR)/Common.c  \
					$(MY_COMMON_DIR)/Base64.c \
					$(MY_COMMON_DIR)/gbk2ucs.c \
					$(MY_COMMON_DIR)/Debug.c\
					$(MY_COMMON_DIR)/LinuxImage.c \
				


LOCAL_C_INCLUDES := ./ \
					$(MY_COMMON_DIR)/ \




 include $(BUILD_SHARED_LIBRARY)     #SO
#include $(BUILD_EXECUTABLE)     	#可执行程序
#include $(BUILD_STATIC_LIBRARY) 	#静态库



