LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libhaywire

LOCAL_SRC_FILES := \
../../lib/haywire/src/haywire/configuration/ini.c \
../../lib/haywire/src/haywire/configuration/configuration.c \
../../lib/haywire/src/haywire/connection_consumer.c \
../../lib/haywire/src/haywire/connection_dispatcher.c \
../../lib/haywire/src/haywire/http_parser.c \
../../lib/haywire/src/haywire/http_request.c \
../../lib/haywire/src/haywire/http_response.c \
../../lib/haywire/src/haywire/http_response_cache.c \
../../lib/haywire/src/haywire/http_server.c \
../../lib/haywire/src/haywire/http_server_impl.c \
../../lib/haywire/src/haywire/hw_string.c \
../../lib/haywire/src/haywire/route_compare_method.c \
../../lib/haywire/src/haywire/server_stats.c 

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../lib/haywire/include \
$(LOCAL_PATH)/../../lib/libuv/include

include $(BUILD_STATIC_LIBRARY)