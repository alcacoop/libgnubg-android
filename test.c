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

void checkMoves() {
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);

  int move [8];
  int board [2][25] = { 
    {0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
    {0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} 
  };
  cubeinfo ci;
  ci.nCube = 1;
  ci.fCubeOwner = -1:
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
  ec.fDeterministic = 1;
  ec.rNoise = 0.060;

  //  printBoard(board);

  FindBestMove(move, 5, 1, board, &ci, NULL, NULL);
  int ii=0;
  for (ii=0;ii<4;ii++){
    printf(" -%d- ", move[ii]);
  }
  printf(" | ");
  for (ii=4;ii<8;ii++){
    printf(" -%d- ", move[ii]);
  }
  printf("\n");

  //  printBoard(board);
};

int main (int argc, char** argv) {
  int d[2];
  unsigned long a;
  int b;
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  rngcontext *_rngctx;
  int i;
  
  _rngctx = InitRNG(&a, &b, TRUE, _rng);
  //  printf("%d %d\n", a,b);
  //rcRollout.nSeed ^= 0x792A584B;
  /* for (i=0; i<10; i++) { */
  /*   RollDice(d, &_rng, _rngctx); */
  /*   printf("LANCIO: %d %d\n", d[0], d[1]); */
  /* } */
  
  //  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);

  int _k;
  for(_k = 0; _k < 30; _k++) {
    checkMoves();
    /* printf("\n"); */
  }
}
