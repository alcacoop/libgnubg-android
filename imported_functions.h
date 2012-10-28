#ifndef IMPORTED_FUNCTIONS_H
#define IMPORTED_FUNCTIONS_H

#include "eval.h"
#include "rollout.h"
#include "backgammon.h"

#ifdef IS_ANDROID
#include <android/log.h>
#endif



#ifdef IS_ANDROID
#define MYLOG(msg) __android_log_write(ANDROID_LOG_ERROR,"MYLOG()",msg);
#else
#define MYLOG(msg) printf(msg)
#endif

extern void ProcessEvents(void);
extern void get_eq_before_resign(cubeinfo *pci, decisionData *pdd);
extern void EvaluateRoll ( float ar[ NUM_ROLLOUT_OUTPUTS ], int nDie1, int nDie2, const TanBoard anBoard, const cubeinfo *pci, const evalcontext *pec);
extern void init_rng(void);
extern void GetMatchStateCubeInfo( cubeinfo* pci, const matchstate* pms );

#endif
