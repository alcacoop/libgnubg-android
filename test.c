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
