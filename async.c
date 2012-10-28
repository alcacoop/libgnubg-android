#include "async.h"



extern int RunAsyncProcess(AsyncFun fn, void *data, const char *msg)
{
	fn(data);	/* Just call function in single threaded build */
	return 0;
}


void asyncCubeDecision(decisionData *pdd)
{
	GeneralCubeDecision(pdd->aarOutput, pdd->aarStdDev, pdd->aarsStatistics,
			pdd->pboard, pdd->pci, pdd->pes, NULL, NULL);
}

void asyncMoveDecisionE(decisionData *pdd)
{
	GeneralEvaluationE(pdd->aarOutput[0], pdd->pboard, pdd->pci, pdd->pec);
}

void asyncEvalRoll(decisionData *pdd)
{
	EvaluateRoll(pdd->aarOutput[0], ms.anDice[0], ms.anDice[1], pdd->pboard, pdd->pci, pdd->pec);
}
