#include "async.h"


extern int asyncRet;

void asyncMoveDecisionE(decisionData *pdd)
{
	if ( GeneralEvaluationE ( pdd->aarOutput[0], pdd->pboard, pdd->pci, pdd->pec) < 0 )
		MT_SetResultFailed();
}

void asyncEvalRoll(decisionData *pdd)
{
	EvaluateRoll (pdd->aarOutput[0], ms.anDice[0], ms.anDice[1], pdd->pboard, pdd->pci, pdd->pec );
}


extern int RunAsyncProcess(AsyncFun fn, void *data, const char *msg)
{
	int ret;
#if USE_MULTITHREAD
	Task *pt = (Task*)malloc(sizeof(Task));
	pt->pLinkedTask = NULL;
	pt->fun = fn;
	pt->data = data;
	MT_AddTask(pt, FALSE);
#endif

	//ProgressStart(msg);
	
#if USE_MULTITHREAD
	ret = MT_WaitForTasks(Progress, 100, FALSE);
#else
	asyncRet = 0;
	fn(data);	/* Just call function in single threaded build */
	ret = asyncRet;
#endif

	//ProgressEnd();

	return ret;
}

void asyncCubeDecision(decisionData *pdd)
{
	if ( GeneralCubeDecision( pdd->aarOutput, pdd->aarStdDev, pdd->aarsStatistics,
			pdd->pboard, pdd->pci, pdd->pes, NULL, NULL ) < 0 )
		MT_SetResultFailed();
}
