LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    gthread-impl.c

LOCAL_STATIC_LIBRARIES := \
	libglib-2.0

LOCAL_MODULE:= libgthread-2.0

LOCAL_C_INCLUDES := 		\
	$(LOCAL_PATH)		\
	$(GLIB_TOP)		\
	$(GLIB_TOP)/glib/android	\
	$(GLIB_TOP)/glib/android-config	\
	$(GLIB_TOP)/glib

LOCAL_CFLAGS := \
    -DG_LOG_DOMAIN=\"GThread\"      \
    -D_POSIX4_DRAFT_SOURCE          \
    -D_POSIX4A_DRAFT10_SOURCE       \
    -U_OSF_SOURCE                   \
    -DG_DISABLE_DEPRECATED  \
    -DGLIB_COMPILATION=1

include $(BUILD_STATIC_LIBRARY)

#
# DUMY
#

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= libgthread-2.0-shared
LOCAL_STATIC_LIBRARIES := \
	libgthread-2.0
include $(BUILD_SHARED_LIBRARY)

