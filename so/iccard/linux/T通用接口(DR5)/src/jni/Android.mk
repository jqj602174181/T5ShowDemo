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

include $(CLEAR_VARS)

#LOCAL_CFLAGS += -DFUNCTION_TEST
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog
LOCAL_MODULE    := IcCard
LOCAL_SRC_FILES := ../ICCardJni.c ../IccReader.c ../IccAppComm.c\
					../../../../common/Application/CpuCard/ECash.c\
					../../../../common/Application/CpuCard/ICC_CardCmd.c\
					../../../../common/CardReaderCmd/CardReaderLayer.c\
					../../../../common/CardReaderCmd/CKB51XX/CKB51XX.c\
					../../../../common/PublicLibrary/Debug.c\
					../../../../common/PublicLibrary/Des.c\
					../../../../common/PublicLibrary/StrOp.c\
					../../../../common/PublicLibrary/TlvOp.c\
					../../../../common/TransMssionLayer/TransMssion.c\
					../../../../common/TransMssionLayer/Linux/TransMssionLinux.c\
					../../../../common/TransMssionLayer/Linux/com/LinuxAux.c\
					../../../../common/TransMssionLayer/Linux/buf/read_fifo.c\
					../../../../common/TransMssionLayer/Linux/buf/queue.c

LOCAL_C_INCLUDES := ../ \
					../../../../common/Application/CpuCard/\
					../../../../common/CardReaderCmd/\
					../../../../common/\
					../../../../common/CardReaderCmd/CKB51XX/\
					../../../../common/PublicLibrary/\
					../../../../common/TransMssionLayer/\
					../../../../common/TransMssionLayer/Linux/\
					../../../../common/TransMssionLayer/Linux/com/\
					../../../../common/TransMssionLayer/Linux/hid/\
					../../../../common/TransMssionLayer/Linux/buf/


include $(BUILD_SHARED_LIBRARY)     #SO
#include $(BUILD_EXECUTABLE)     	#ø…÷¥––≥Ã–Ú
#include $(BUILD_STATIC_LIBRARY) 	#æ≤Ã¨ø‚