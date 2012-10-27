#include "async.h"


extern int asyncRet;

extern int RunAsyncProcess(AsyncFun fn, void *data, const char *msg)
{
	int ret;
	asyncRet = 0;
	fn(data);	/* Just call function in single threaded build */
	ret = asyncRet;
	return ret;
}


void asyncCubeDecision(decisionData *pdd)
{
	if ( GeneralCubeDecision( pdd->aarOutput, pdd->aarStdDev, pdd->aarsStatistics,
			pdd->pboard, pdd->pci, pdd->pes, NULL, NULL ) < 0 )
		MT_SetResultFailed();
}

void asyncMoveDecisionE(decisionData *pdd)
{
	if ( GeneralEvaluationE ( pdd->aarOutput[0], pdd->pboard, pdd->pci, pdd->pec) < 0 )
		MT_SetResultFailed();
}

void asyncEvalRoll(decisionData *pdd)
{
	EvaluateRoll(pdd->aarOutput[0], ms.anDice[0], ms.anDice[1], pdd->pboard, pdd->pci, pdd->pec);
}
