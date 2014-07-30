#ifndef ASYNC_H
#define ASYNC_H

#include "backgammon.h"

#define MT_SetResultFailed() asyncRet = -1


void asyncMoveDecisionE(decisionData *pdd);
void asyncEvalRoll(decisionData *pdd);
extern int RunAsyncProcess(AsyncFun fn, void *data, const char *msg);
void asyncCubeDecision(decisionData *pdd);

#endif
