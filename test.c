#include "globals.h"
#include "async.h"


extern void ProcessEvents(void)
{
	{
		while (g_main_context_pending(NULL))
			g_main_context_iteration(NULL, TRUE);
	}
}


//DOM0: VERIFICA RESIGN
static void get_eq_before_resign(cubeinfo *pci, decisionData *pdd)
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


static void init_rng(void)
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


void printBoard(int board [2][25]) {
  int i, j;
  for(i = 0; i<2; i++) {
    for(j=0; j<25; j++) {
      printf(" %u ", board[i][j]);
    }
    printf("\n");
  }
}

void printMove(int move[8]){
  int i=0;
  for (i=0;i<4;i++){
    printf("\t%d", move[i]);
  }
  printf("\t|");
  for (i=4;i<8;i++){
    printf("\t%d", move[i]);
  }
  printf("\n");
}


void checkMoves() {
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  int move [8];
  cubeinfo ci;
  int *board = ms.anBoard;

  GetMatchStateCubeInfo( &ci, &ms );
  FindBestMove(move, 5, 1, board, &ci, &ec, mf);
  printMove(move);
  //printBoard(board);
};


void rollDice(int d[2]) {
  unsigned long a;
  int b;
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  rngcontext *_rngctx;
  
  //_rngctx = InitRNG(&a, &b, TRUE, _rng);
  RollDice(d, &_rng, rngctxCurrent); 
}



int acceptResign() {
  int *board = ms.anBoard;
  cubeinfo ci;
  GetMatchStateCubeInfo( &ci, &ms );

  float rEqBefore, rEqAfter;
  const float max_gain = 1e-6f;
  decisionData dd;
  int resign;


  get_eq_before_resign(&ci, &dd);
  getResignEquities(dd.aarOutput[0], &ci, ms.fResigned, &rEqBefore, &rEqAfter);
  if (rEqAfter - rEqBefore < max_gain )
    resign = ms.fResigned;
  else
    resign = -1;

  if (resign > 0)
  {
    ms.fResigned = resign;
    printf("Resign decision: ACCEPTED!\n");
    
  }
  else
  {
    printf("Resign decision: RESIGN!\n");
  }

}




int main (int argc, char** argv) {
  init_rng();
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  int _k;
  for(_k = 0; _k < 1; _k++) {
    checkMoves();
  }
  acceptResign();
}
