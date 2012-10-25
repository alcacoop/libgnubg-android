#include "dice.h"
#include "eval.h"

#define TRUE 1

/*
  int board [2][25] = { 
  {0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
  {0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} 
  };
*/

int board [2][25] = { 
  {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
  {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}
};

cubeinfo ci = {
  .nCube = 1,
  .fCubeOwner = -1,
  .fMove = 0,
  .nMatchTo = 1,
  .anScore[0] = 0,
  .anScore[1] = 0,
  .fCrawford = 0,
  .fJacoby = 1,
  .fBeavers = 3,
  .bgv = VARIATION_STANDARD
};

//CONFIGURAZIONE SUPREMO
evalcontext ec = {
  .fCubeful = 1,
  .nPlies = 3,
  .fUsePrune = 1,
  .fDeterministic = 1,
  .rNoise = 0.000
};

movefilter mf[] = {
  {0, 16, 0.32000},
  {0, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 16, 0.32000},
  {-1, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 16, 0.32000},
  {-1, 0, 0.00000},
  {0, 4, 0.08000},
  {0, 0, 0.00000},
  {0, 16, 0.32000},
  {-1, 0, 0.00000},
  {0, 4, 0.08000},
  {-1, 0, 0.00000}
};

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
  
  _rngctx = InitRNG(&a, &b, TRUE, _rng);
  //rcRollout.nSeed ^= 0x792A584B;
  RollDice(d, &_rng, _rngctx); 
}

void cubeDecision() {
  float aarOutput[ 2 ][ NUM_ROLLOUT_OUTPUTS ];
  
  int r = GeneralCubeDecisionE(aarOutput, board, &ci, &ec, NULL);
  printf("Cube decision: %d\n", r);
}

int main (int argc, char** argv) {
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  int _k;
  for(_k = 0; _k < 1; _k++) {
    checkMoves();
  }
  cubeDecision();
}
