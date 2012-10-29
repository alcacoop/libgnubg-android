#ifndef GLOBALS_H
#define GLOBALS_H

#include "eval.h"

#define TRUE 1
#define FALSE 0
#define EXIT_FAILURE -1

/* this is the "normal" movefilter*/
#define MOVEFILTER \
  { { { 0,  8, 0.16f }, {  0, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, {  0, 0, 0 } }, \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, { -1, 0, 0 } } , \
  }

#ifdef IS_ANDROID
#define DATA_DIR  "/sdcard/gnubg/"
#else
#define DATA_DIR  "./"
#endif

#undef BuildFilename
extern char* BuildFilename(char* str);


#ifdef IS_ANDROID
#include <android/log.h>
#define MYLOG(msg) __android_log_write(ANDROID_LOG_ERROR,"MYLOG()",msg);
#else
#define MYLOG(msg) printf(msg)
#endif

extern evalcontext ec;
extern movefilter mf[4][4];

#endif

