#include "globals.h"
#include "async.h"
#include "imported_functions.h"

//CONFIGURAZIONE SUPREMO
extern evalcontext ec;
extern movefilter mf[];

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
  
  RollDice(d, &_rng, rngctxCurrent); 
}

extern int check_resigns(cubeinfo * pci)
{
  float rEqBefore, rEqAfter;
  const float max_cost = 0.05f;
  const float max_gain = 1e-6f;
  decisionData dd;
  cubeinfo ci;
  int resigned = 1;

  if (pci == NULL)
    {
      GetMatchStateCubeInfo( &ci, &ms );
      pci = &ci;
    }

  get_eq_before_resign(pci, &dd);
  do
    {
      getResignEquities(dd.aarOutput[0], pci, resigned, &rEqBefore, &rEqAfter);
      if (rEqBefore - rEqAfter > max_cost)
        {
          resigned=4;
          break;
        }
      else if (rEqAfter - rEqBefore < max_gain )
        break;
    }
  while (resigned++ < 3);
  return resigned == 4 ? -1 : resigned;
}

//TODO: move to API
int acceptResign() {
  int *board = ms.anBoard;
  cubeinfo ci;
  GetMatchStateCubeInfo( &ci, &ms );
  
  int resign;
  if (ms.fResigned == -1) {
    printf("RESIGNED = -1\n");
    resign = check_resigns(&ci);
  }
  else {
    float rEqBefore, rEqAfter;
    const float max_gain = 1e-6f;
    decisionData dd;
    
    get_eq_before_resign(&ci, &dd);
    getResignEquities(dd.aarOutput[0], &ci, ms.fResigned, &rEqBefore, &rEqAfter);
    if (rEqAfter - rEqBefore < max_gain ) {
      resign = ms.fResigned;
      printf("QUALCOSA %d\n", resign);
    }
    else {
      resign = -1;
      printf("QUALCOSA2 %d\n", resign);
    }
  }

  if (resign > 0)
    {
      ms.fResigned = resign;
      printf("Resign decision: ACCEPTED!\n");    
    }
  else
    {
      printf("Resign decision: NOT ACCEPTED!\n");
    }

}

void acceptDouble() {
  decisionData dd;
  cubedecision cd;
  cubeinfo ci;
  float arDouble[4];
  evalsetup es;
  es.et = EVAL_EVAL;
  es.ec = ec;
  es.rc = rcRollout;

  /* Consider cube action */

  /* 
   * We may get here in three different scenarios: 
   * (1) normal double by opponent: fMove != fTurn and fCubeOwner is
   *     either -1 (centered cube) or = fMove.
   * (2) beaver by opponent: fMove = fTurn and fCubeOwner = !
   *     fMove
   * (3) raccoon by opponent: fMove != fTurn and fCubeOwner =
   *     fTurn.
   *
   */

  if ( ms.fMove != ms.fTurn && ms.fCubeOwner == ms.fTurn ) {

    /* raccoon: consider this a normal double, i.e. 
       fCubeOwner = fMove */
        
    SetCubeInfo ( &ci, ci.nCube,
                  ci.fMove, ci.fMove,
                  ci.nMatchTo, ci.anScore, ci.fCrawford,
                  ci.fJacoby, ci.fBeavers, ci.bgv );

  }
      
  if ( ms.fMove == ms.fTurn && ms.fCubeOwner != ms.fMove ) {

    /* opponent beavered: consider this a normal double by me */

    SetCubeInfo ( &ci, ci.nCube,
                  ci.fMove, ci.fMove,
                  ci.nMatchTo, ci.anScore, ci.fCrawford,
                  ci.fJacoby, ci.fBeavers, ci.bgv );

  }

  /* Evaluate cube decision */
  dd.pboard = ms.anBoard;
  dd.pci = &ci;
  dd.pes = &es;
  if (RunAsyncProcess((AsyncFun)asyncCubeDecision, &dd, _("Considering cube action...")) != 0)
    return -1;

  current_pmr_cubedata_update(dd.pes, dd.aarOutput, dd.aarStdDev);

  cd = FindCubeDecision ( arDouble,  dd.aarOutput, &ci );

  if ( ms.fTurn == ms.fMove ) {

    /* opponent has beavered */

    switch ( cd ) {

    case DOUBLE_TAKE:
    case REDOUBLE_TAKE:
    case TOOGOOD_TAKE:
    case TOOGOODRE_TAKE:
    case DOUBLE_PASS:
    case TOOGOOD_PASS:
    case REDOUBLE_PASS:
    case TOOGOODRE_PASS:
    case OPTIONAL_DOUBLE_TAKE:
    case OPTIONAL_REDOUBLE_TAKE:
    case OPTIONAL_DOUBLE_PASS:
    case OPTIONAL_REDOUBLE_PASS:

      /* Opponent out of his right mind: Raccoon if possible */

      if ( ms.cBeavers < nBeavers && ! ms.nMatchTo &&
           ms.nCube < ( MAX_CUBE >> 1 ) ) {}
        /* he he: raccoon */
        // CommandRedouble ( NULL );
      else {}
        /* Damn, no raccoons allowed */
        // CommandTake ( NULL );

      break;

          
    case NODOUBLE_TAKE:
    case NO_REDOUBLE_TAKE:

      /* hmm, oops: opponent beavered us:
         consider dropping the beaver */

      /* Note, this should not happen as the computer plays
         "perfectly"!! */

      if ( arDouble[ OUTPUT_TAKE ] <= -1.0 ) {}
        /* drop beaver */
        // CommandDrop ( NULL );
      else {}
        /* take */
        // CommandTake ( NULL );
          
      break;


    case DOUBLE_BEAVER:
    case NODOUBLE_BEAVER:
    case NO_REDOUBLE_BEAVER:
    case OPTIONAL_DOUBLE_BEAVER:

      /* opponent beaver was correct */

      // CommandTake ( NULL );
      break;

    default:

      g_assert ( FALSE );
          
    } /* switch cubedecision */

  } /* consider beaver */
  else {

    /* normal double by opponent */

    switch ( cd ) {
        
    case DOUBLE_TAKE:
    case NODOUBLE_TAKE:
    case TOOGOOD_TAKE:
    case REDOUBLE_TAKE:
    case NO_REDOUBLE_TAKE:
    case TOOGOODRE_TAKE:
    case NODOUBLE_DEADCUBE:
    case NO_REDOUBLE_DEADCUBE:
    case OPTIONAL_DOUBLE_TAKE:
    case OPTIONAL_REDOUBLE_TAKE:

      // CommandTake ( NULL );
      break;

    case DOUBLE_PASS:
    case TOOGOOD_PASS:
    case REDOUBLE_PASS:
    case TOOGOODRE_PASS:
    case OPTIONAL_DOUBLE_PASS:
    case OPTIONAL_REDOUBLE_PASS:

      // CommandDrop ( NULL );
      break;

    case DOUBLE_BEAVER:
    case NODOUBLE_BEAVER:
    case NO_REDOUBLE_BEAVER:
    case OPTIONAL_DOUBLE_BEAVER:

      if ( ms.cBeavers < nBeavers && ! ms.nMatchTo &&
           ms.nCube < ( MAX_CUBE >> 1 ) ) {}
        /* Beaver all night! */
        // CommandRedouble ( NULL );
      else {}
        /* Damn, no beavers allowed */
        // CommandTake ( NULL );

      break;

    default:

      g_assert ( FALSE );

    } /* switch cubedecision */

  } /* normal cube */
      
}

void testResignation() {
  unsigned int b[2][25] = 
    {
      {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {13, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  printBoard(ms.anBoard);
  ms.fResigned = 2;
  // fTurn = chi deve decidere (accettare double/resign: 0: umano; 1: pc)
  // fMove = chi deve lanciare i dadi (0: umano; 1: pc)
  ms.fMove = 1;
  ms.fTurn = 0;
  cubeinfo ci;
  GetMatchStateCubeInfo( &ci, &ms );
  acceptResign();
}

int main (int argc, char** argv) {
  init_rng();
  InitMatchEquity("/usr/share/gnubg/met/zadeh.xml");
  int d[2];
  rollDice(d);
  printf("%d %d\n", d[0], d[1]);
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  //  checkMoves();
  testResignation();
}
