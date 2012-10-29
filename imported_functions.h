#ifndef IMPORTED_FUNCTIONS_H
#define IMPORTED_FUNCTIONS_H

#include "eval.h"
#include "rollout.h"
#include "backgammon.h"


extern void ProcessEvents(void);
extern void get_eq_before_resign(cubeinfo *pci, decisionData *pdd);
extern void EvaluateRoll ( float ar[ NUM_ROLLOUT_OUTPUTS ], int nDie1, int nDie2, const TanBoard anBoard, const cubeinfo *pci, const evalcontext *pec);
extern void init_rng(void);
extern void GetMatchStateCubeInfo( cubeinfo* pci, const matchstate* pms );

#endif
