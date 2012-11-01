include $(CLEAR_VARS)  

LOCAL_PATH := $(call my-dir)  
LOCAL_PATH :=/home/dmt/Progetti/Android/workspace/TestNDK/jni/
GLIB_TOP := $(LOCAL_PATH)/glib/

include $(call all-subdir-makefiles)

include $(GLIB_TOP)glib/Android.mk
include $(GLIB_TOP)gthread/Android.mk


LOCAL_PATH :=/home/dmt/Progetti/Android/workspace/TestNDK/jni/
LOCAL_SRC_FILES := \
    lib/list.c \
    lib/neuralnet.c \
    lib/mt19937ar.c \
    lib/isaac.c \
    lib/md5.c \
    lib/cache.c \
    lib/inputs.c \
    globals.c \
    async.c \
    dice.c \
    eval.c \
    bearoff.c \
    bearoffgammon.c \
    imported_functions.c \
		jniAPI.c \
    levels.c \
    matchequity.c \
    matchid.c \
    nativeAPI.c \
    util.c \
    mec.c \
    positionid.c \
    rollout.c \
    test.c


LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_MODULE := libgnubg
LOCAL_MODULE_FILENAME := libgnubg

LOCAL_SHARED_LIBRARIES := libgthread-2.0 libglib-2.0


GLIB_C_INCLUDES :=			\
	$(GLIB_TOP)			\
	$(GLIB_TOP)glib/glib		\
	$(GLIB_TOP)glib/android \
	$(GLIB_TOP)glib/android-internal

LOCAL_CFLAGS=-DIS_LIBRARY -DIS_ANDROID -DHAVE_CONFIG_H -g -O2 -DLOCALEDIR=\"/usr/share/locale\" -pthread -DAC_DATADIR=\"/usr/share\" -DAC_PKGDATADIR=\"/usr/share/gnubg\" -DAC_DOCDIR=\"/usr/share/doc/gnubg/\" -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -w

LOCAL_C_INCLUDES :=			\
	$(GLIB_TOP)			\
	$(LOCAL_PATH)lib 	\
	$(GLIB_TOP)glib		\
	$(GLIB_TOP)android-internal \
	$(GLIB_TOP)android

GTHREAD_C_INCLUDES :=			\
	$(GLIB_C_INCLUDES)		\
	$(GLIB_TOP)gthread

GLIB_SHARED_LIBRARIES := \
	libgthread-2.0 \
	libglib-2.0

include $(BUILD_SHARED_LIBRARY)  


