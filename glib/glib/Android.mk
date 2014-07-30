LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#remove, burke
#gcache.c        \
#gcompletion.c   \
#    grel.c          \
#gbuffer.c \


    # gtestutils.c    
			
LOCAL_SRC_FILES:= \
    ./libcharset/localcharset.c \
    gdatetime.c     \
    gtestutils.c    \
    gtimezone.c     \
    gurifuncs.c     \
    ghostutils.c    \
    garray.c        \
    gchecksum.c     \
    gbitlock.c      \
    gpoll.c         \
    gasyncqueue.c   \
    gatomic.c       \
    gbacktrace.c    \
    gbase64.c       \
    gbookmarkfile.c \
	\
    gconvert.c      \
    gdataset.c      \
    gdate.c         \
    gdir.c          \
    gerror.c        \
    gfileutils.c    \
    ghash.c         \
    ghook.c         \
    giochannel.c    \
    gkeyfile.c      \
    glist.c         \
    gmain.c         \
    gmappedfile.c   \
    gmarkup.c       \
    gmem.c          \
    gmessages.c     \
    gnode.c         \
    goption.c       \
    gpattern.c      \
    gprimes.c       \
    gqsort.c        \
    gqueue.c        \
	\
    grand.c         \
    gscanner.c      \
    gsequence.c     \
    gshell.c        \
    gslice.c        \
    gslist.c        \
    gstdio.c        \
    gstrfuncs.c     \
    gstring.c       \
    gthread.c       \
    deprecated/gthread-deprecated.c       \
    gthreadpool.c   \
    gtimer.c        \
    gtree.c         \
    guniprop.c      \
    gutf8.c         \
    gunibreak.c     \
    gunicollate.c   \
    gunidecomp.c    \
    gutils.c        \
    gprintf.c       \
    giounix.c       \
    gvariant.c      \
    gvariant-core.c \
    gvariant-parser.c \
    gvariant-serialiser.c \
    gvarianttype.c \
    gvarianttypeinfo.c \
    \
    gspawn.c \
	\
	gthread-posix.c \
	gwakeup.c \
	gcharset.c \
	glib-init.c \
	gquark.c \
	genviron.c \
	glib-unix.c \
	gregex.c \
	glib-private.c \
	ggettext.c \
	gbytes.c

LOCAL_MODULE:= libglib-2.0

LOCAL_C_INCLUDES := 			\
	$(GLIB_TOP)			\
	$(GLIB_TOP)/glib/android/		\
	$(GLIB_TOP)/glib/android-config/		\
	$(GLIB_TOP)/glib/		\
	$(LOCAL_PATH)/libcharset        \
	$(LOCAL_PATH)/gnulib            \
	$(LOCAL_PATH)/pcre \
	$(LOCAL_PATH)/../../zlib


LOCAL_STATIC_LIBRARIES := \
	$(LOCAL_PATH)/../../libs/libpcre.a \
	$(LOCAL_PATH)/../../libs/libpcreposix.a

LOCAL_SHARED_LIBRARIES := \
	-lz

# ./glib private macros, copy from Makefile.am
LOCAL_CFLAGS := \
    -DLIBDIR=\"$(libdir)\"          \
    -DHAVE_CONFIG_H                 \
    \
    -DG_LOG_DOMAIN=\"GLib-GRegex\" \
    -DSUPPORT_UCP \
    -DSUPPORT_UTF8 \
    -DNEWLINE=-1 \
    -DMATCH_LIMIT=10000000 \
    -DMATCH_LIMIT_RECURSION=10000000 \
    -DMAX_NAME_SIZE=32 \
    -DMAX_NAME_COUNT=10000 \
    -DMAX_DUPLENGTH=30000 \
    -DLINK_SIZE=2 \
    -DEBCDIC=0 \
    -DPOSIX_MALLOC_THRESHOLD=10 \
    -DG_DISABLE_DEPRECATED \
    -DGLIB_COMPILATION

include $(BUILD_STATIC_LIBRARY)

#################
# DUMY
#################

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:= libglib-2.0-shared
LOCAL_STATIC_LIBRARIES := libglib-2.0

include $(BUILD_SHARED_LIBRARY)
