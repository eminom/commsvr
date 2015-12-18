LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libcommsvr

LOCAL_MODULE_FILENAME := libcommsvr-static-android
#LOCAL_MODULE_FILENAME := libcommsvr-shared-android

LOCAL_SRC_FILES := \
../src/client_proc_t.cc \
../src/services/Inception.cc \
../src/routes/HelloInception.cc \
../src/routes/HiFetch.cc \
../src/routes/HiUpload.cc \
../src/routes/HiResources.cc \
../src/routes/HiIndex.cc \
../src/routes/task/TaskUVBase.cc \
../src/routes/task/UploadTask.cc \
../src/files/DirWalker.cc \
../src/files/RootExplorer.cc \
../src/files/DirMaker.cc \
../src/config/HttpServerConfig.cc \
../src/utils/StreamBuffer.cc \
../src/utils/ResponseUtils.cc \
../src/utils/CmdBuffStr.cc 

LOCAL_STATIC_LIBRARIES += libuv
LOCAL_STATIC_LIBRARIES += libhaywire

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../src \
$(LOCAL_PATH)/../lib/libuv/include \
$(LOCAL_PATH)/../lib/libuv/src \
$(LOCAL_PATH)/../lib/haywire/include

include $(BUILD_STATIC_LIBRARY)
$(call import-module, commsvr/build/libuv.android)
$(call import-module, commsvr/build/haywire.android)