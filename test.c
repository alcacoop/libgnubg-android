#include "globals.h"
#include "async.h"
#include "imported_functions.h"

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
    if (rEqAfter - rEqBefore < max_gain )
      resign = ms.fResigned;
    else
      resign = -1;
  }

  if (resign > 0) {
    ms.fResigned = resign;
    printf("RESIGN ACCEPTED!\n");
  }
  else
    printf("RESIGN NOT ACCEPTED!\n");

}



//TODO: move to API
void acceptDouble() {
  decisionData dd;
  cubedecision cd;
  cubeinfo ci;
  float arDouble[4];
  evalsetup es;
  es.et = EVAL_EVAL;
  es.ec = ec;
  es.rc = rcRollout;

  GetMatchStateCubeInfo( &ci, &ms );

  /* Evaluate cube decision */
  dd.pboard = ms.anBoard;
  dd.pci = &ci;
  dd.pes = &es;
  if (RunAsyncProcess((AsyncFun)asyncCubeDecision, &dd, "Considering cube action...") != 0)
    return -1;
  cd = FindCubeDecision ( arDouble,  dd.aarOutput, &ci );

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
      printf("DOUBLE ACCEPTED!\n");
      break;

    case DOUBLE_PASS:
    case TOOGOOD_PASS:
    case REDOUBLE_PASS:
    case TOOGOODRE_PASS:
    case OPTIONAL_DOUBLE_PASS:
    case OPTIONAL_REDOUBLE_PASS:
      // CommandDrop ( NULL );
      printf("DOUBLE NOT ACCEPTED!\n");
      break;

    case DOUBLE_BEAVER:
    case NODOUBLE_BEAVER:
    case NO_REDOUBLE_BEAVER:
    case OPTIONAL_DOUBLE_BEAVER:
      // CommandTake ( NULL );
      printf("DOUBLE ACCEPTED!\n");
      break;

    default:
      g_assert ( FALSE );
  } 
}



void testResignation() {
  unsigned int b[2][25] = 
    {
      {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
    };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  ms.fResigned = 2;
  // fTurn = chi deve decidere (accettare double/resign: 0: umano; 1: pc)
  // fMove = chi deve lanciare i dadi (0: umano; 1: pc)
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  //printBoard(ms.anBoard);
  acceptResign();
}



void testDoubling() {
  unsigned int b[2][25] = 
    {
      {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
    };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  ms.fDoubled = 1;
  ms.nCube = 4;
  ms.fCubeOwner = -1;
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  //printBoard(ms.anBoard);
  acceptDouble();
}




int main (int argc, char** argv) {
  init_rng();
  InitMatchEquity("zadeh.xml");
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);

  int d[2];

  rollDice(d);
  printf("%d %d\n", d[0], d[1]);

  //checkMoves();
  
  testResignation();
  testDoubling();
}
