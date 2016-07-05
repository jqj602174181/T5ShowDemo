#!/bin/sh

ndk-build  NDK_PROJECT_PATH=.  APP_BUILD_SCRIPT=Android.mk
rm -rf ./obj
#adb push libs/armeabi/ICCard /tmp/test


