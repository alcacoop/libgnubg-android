#include "dice.h"
#include "eval.h"
#define TRUE 1

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
  cubeinfo ci;
  ci.nCube = 1;
  ci.fCubeOwner = -1;
  ci.fMove = 0;
  ci.nMatchTo = 1;
  ci.anScore[0] = 0;
  ci.anScore[1] = 0;
  ci.fCrawford = 0;
  ci.fJacoby = 1;
  ci.fBeavers = 3;
  ci.bgv = VARIATION_STANDARD;

  evalcontext ec;
  ec.fCubeful = 1;
  ec.nPlies = 0;
  ec.fUsePrune = 0;
  ec.fDeterministic = 0;
  ec.rNoise = 0.060;

  FindBestMove(move, 3, 1, board, &ci, &ec, NULL);
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



int main (int argc, char** argv) {
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  int _k;
  for(_k = 0; _k < 10; _k++) {
    checkMoves();
  }
}
