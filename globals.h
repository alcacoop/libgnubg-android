#ifndef GLOBALS_H
#define GLOBALS_H

#include "eval.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE -1
#endif

/* this is the "normal" movefilter*/
#define MOVEFILTER \
  { { { 0,  8, 0.16f }, {  0, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, {  0, 0, 0 } }, \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, { -1, 0, 0 } } , \
  }

#define BACKGAMMON_BOARD \
  {{0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}, \
      {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}}

#define NACKGAMMON_BOARD \
  {{0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0}, \
      {0, 0, 0, 0, 0, 4, 0, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0}}

#ifdef IS_ANDROID
#define DATA_DIR  "/sdcard/gnubg/"
#else
#define DATA_DIR  "./"
#endif

#undef BuildFilename
extern char* BuildFilename(char* str);
extern void MYLOG(char*);


extern matchstate ms;
extern evalcontext ec;
extern movefilter mf[4][4];
extern rolloutcontext rcRollout;
extern int fOutputMWC;
extern int fShowProgress;
extern int fAutoSaveRollout;
extern int fAutoCrawford;
extern rngcontext *rngctxRollout;

extern ConstTanBoard msBoard();

#endif

