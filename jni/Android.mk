LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libcommsvr

LOCAL_MODULE_FILENAME := libcommsvr-static-android
#LOCAL_MODULE_FILENAME := libcommsvr-shared-android

LOCAL_SRC_FILES := \
../src/client_proc_t.cc \
../src/Inception.cc \
../src/StreamBuffer.cc 

LOCAL_STATIC_LIBRARIES += libuv

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../lib/libuv/include \
$(LOCAL_PATH)/../lib/libuv/src \

include $(BUILD_STATIC_LIBRARY)
$(call import-module, commsvr/build/libuv.android)