#include "imported_functions.h"
#include "globals.h"

extern void ProcessEvents(void)
{
  {
    while (g_main_context_pending(NULL))
      g_main_context_iteration(NULL, TRUE);
  }
}

extern void get_eq_before_resign(cubeinfo *pci, decisionData *pdd)
{
  const evalcontext ecResign = { FALSE, 2, FALSE, TRUE, 0.0 };

  pdd->pboard = ms.anBoard;
  pdd->pci = pci;
  pdd->pec = &ecResign;

  if (ms.anDice[0] > 0) {
    float t;
    /* Opponent has rolled the dice and then resigned. We want to find out if the resignation is OK after
       the roll */
    RunAsyncProcess((AsyncFun) asyncEvalRoll, pdd, _("Considering resignation..."));
    /* Swap the equities as evaluation is for other player */
    pdd->aarOutput[0][OUTPUT_WIN] = 1 - pdd->aarOutput[0][OUTPUT_WIN];
    t = pdd->aarOutput[0][OUTPUT_WINGAMMON];
    pdd->aarOutput[0][OUTPUT_WINGAMMON] = pdd->aarOutput[0][OUTPUT_LOSEGAMMON];
    pdd->aarOutput[0][OUTPUT_LOSEGAMMON] = t;
    t = pdd->aarOutput[0][OUTPUT_WINBACKGAMMON];
    pdd->aarOutput[0][OUTPUT_WINBACKGAMMON] = pdd->aarOutput[0][OUTPUT_LOSEBACKGAMMON];
    pdd->aarOutput[0][OUTPUT_LOSEBACKGAMMON] = t;
  } else {
    RunAsyncProcess((AsyncFun) asyncMoveDecisionE, pdd, _("Considering resignation..."));
  }
}

extern void EvaluateRoll ( float ar[ NUM_ROLLOUT_OUTPUTS ], int nDie1, int nDie2, const TanBoard anBoard, 
                           const cubeinfo *pci, const evalcontext *pec) {
  TanBoard anBoardTemp;
  cubeinfo ciOpp;

  memcpy ( &ciOpp, pci, sizeof ( cubeinfo ) );
  ciOpp.fMove = ! pci->fMove;

  memcpy( &anBoardTemp[ 0 ][ 0 ], &anBoard[ 0 ][ 0 ], 2 * 25 * sizeof( int ) );

  if( FindBestMove( NULL, nDie1, nDie2, anBoardTemp,
                    (cubeinfo *) pci, NULL, defaultFilters ) < 0 )
    return;


  SwapSides( anBoardTemp );

  if ( GeneralEvaluationE( ar, (ConstTanBoard)anBoardTemp, &ciOpp, (evalcontext *) pec ) )
    return;

}

extern void init_rng(void)
{
  if (!(rngctxCurrent = InitRNG(NULL, NULL, TRUE, rngCurrent))) {
    printf("%s\n", _("Failure setting up RNG"));
    exit(EXIT_FAILURE);
  }
  if (!(rngctxRollout = InitRNG(&rcRollout.nSeed, NULL,
                                TRUE, rcRollout.rngRollout))) {
    printf("%s\n", _("Failure setting up RNG for rollout."));
    exit(EXIT_FAILURE);
  }
  /* we don't want rollouts to use the same seed as normal dice (which
     could happen if InitRNG had to use the current time as a seed) -- mix
     it up a little bit */
  rcRollout.nSeed ^= 0x792A584B;
}

extern void GetMatchStateCubeInfo( cubeinfo* pci, const matchstate* pms )
{
  SetCubeInfo( pci, pms->nCube, pms->fCubeOwner, pms->fMove,
               pms->nMatchTo, pms->anScore, pms->fCrawford,
               pms->fJacoby, 3, pms->bgv );
}

extern void current_pmr_cubedata_update(evalsetup *pes, float output[][NUM_ROLLOUT_OUTPUTS], float stddev[][NUM_ROLLOUT_OUTPUTS])
{
	moverecord *pmr = get_current_moverecord(NULL);
	if (!pmr)
		return;
	if (pmr->CubeDecPtr->esDouble.et == EVAL_NONE)
		pmr_cubedata_set(pmr, pes, output, stddev);
}

extern moverecord *get_current_moverecord(int *pfHistory)
{
	if (plLastMove && plLastMove->plNext && plLastMove->plNext->p) {
		if (pfHistory)
			*pfHistory = TRUE;
		return plLastMove->plNext->p;
	}

	if (pfHistory)
		*pfHistory = FALSE;

	if (ms.gs != GAME_PLAYING) {
		pmr_hint_destroy();
		return NULL;
	}

	/* invalidate on changed dice */
	if (ms.anDice[0] > 0 && pmr_hint && pmr_hint->anDice[0] > 0 
	    && (pmr_hint->anDice[0] != ms.anDice[0]
		|| pmr_hint->anDice[1] != ms.anDice[1]))
		pmr_hint_destroy();

	if (!pmr_hint) {
		pmr_hint = NewMoveRecord();
		pmr_hint->fPlayer = ms.fTurn;
	}

	if (ms.anDice[0] > 0) {
		pmr_hint->mt = MOVE_NORMAL;
		pmr_hint->anDice[0] = ms.anDice[0];
		pmr_hint->anDice[1] = ms.anDice[1];
	} else if (ms.fDoubled) {
		pmr_hint->mt = MOVE_TAKE;
	} else {
		pmr_hint->mt = MOVE_DOUBLE;
		pmr_hint->fPlayer = ms.fTurn;
	}
	return pmr_hint;
}
