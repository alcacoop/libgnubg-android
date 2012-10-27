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
  if (!rngctxCurrent) {
    rngctxCurrent = InitRNG(NULL, NULL, TRUE, rngCurrent);
    rngctxRollout = InitRNG(&rcRollout.nSeed, NULL, TRUE, rcRollout.rngRollout);
    rcRollout.nSeed ^= 0x792A584B;
  }
}


extern void GetMatchStateCubeInfo( cubeinfo* pci, const matchstate* pms )
{
  SetCubeInfo( pci, pms->nCube, pms->fCubeOwner, pms->fMove,
               pms->nMatchTo, pms->anScore, pms->fCrawford,
               pms->fJacoby, 3, pms->bgv );
}

