LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libuv

# LOCAL_MODULE_FILENAME := libuv

LOCAL_SRC_FILES := \
../../lib/libuv/src/fs-poll.c \
../../lib/libuv/src/inet.c \
../../lib/libuv/src/threadpool.c \
../../lib/libuv/src/uv-common.c \
../../lib/libuv/src/version.c \
../../lib/libuv/src/unix/android-ifaddrs.c \
../../lib/libuv/src/unix/async.c \
../../lib/libuv/src/unix/core.c \
../../lib/libuv/src/unix/dl.c \
../../lib/libuv/src/unix/fs.c \
../../lib/libuv/src/unix/getaddrinfo.c \
../../lib/libuv/src/unix/getnameinfo.c \
../../lib/libuv/src/unix/linux-core.c \
../../lib/libuv/src/unix/linux-inotify.c \
../../lib/libuv/src/unix/linux-syscalls.c \
../../lib/libuv/src/unix/loop-watcher.c \
../../lib/libuv/src/unix/loop.c \
../../lib/libuv/src/unix/pipe.c \
../../lib/libuv/src/unix/poll.c \
../../lib/libuv/src/unix/process.c \
../../lib/libuv/src/unix/proctitle.c \
../../lib/libuv/src/unix/pthread-fixes.c \
../../lib/libuv/src/unix/signal.c \
../../lib/libuv/src/unix/stream.c \
../../lib/libuv/src/unix/tcp.c \
../../lib/libuv/src/unix/thread.c \
../../lib/libuv/src/unix/timer.c \
../../lib/libuv/src/unix/tty.c \
../../lib/libuv/src/unix/udp.c 

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../lib/libuv/include \
$(LOCAL_PATH)/../../lib/libuv/src \

include $(BUILD_STATIC_LIBRARY)