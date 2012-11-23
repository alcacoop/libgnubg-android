#include "nativeAPI.h"

//UTILS: PRINT A BOARD
void printBoard(ConstTanBoard board) {
  int i, j;
  for(i = 0; i<2; i++) {
    for(j=0; j<25; j++) {
      printf(" %d ", board[i][j]);
    }
    printf("\n");
  }
}


//UTILS: PRINT A MOVE
void printMove(int move[8]) {
  int i=0;
  printf("MOVE: ");
  printf("%d/%d ", move[0], move[1]);
  printf("%d/%d ", move[2], move[3]);
  printf(" | %d/%d ", move[4], move[5]);
  printf("%d/%d", move[6], move[7]);
  printf("\n");
}


//UTILS: PRINT A COUPLE OF DICES
void printDices(int dices[2]) {
  int i=0;
  printf("DICES: %d %d\n", dices[0], dices[1]);
}


void testResignation() {
  int b[2][25] = 
  {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
  };
  setBoard((ConstTanBoard)b);
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.nCube = -1;
  ms.fCubeOwner = 1;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  printf("\n\nTEST ACCETTAZIONE RESIGN...\n");
  printBoard(msBoard());
  printf("RESIGN 1: %s\n", acceptResign(1)?"OK":"NO");
  printf("RESIGN 2: %s\n", acceptResign(2)?"OK":"NO");
  printf("RESIGN 3: %s\n", acceptResign(3)?"OK":"NO");
  printf("AIlevel: %d\n\n", currentAILevel);
}


void testDoubling() {
  int b[2][25] = 
  {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0}
  };

  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  setBoard((ConstTanBoard)b);
  printf("\n\nTEST ACCETTAZIONE DOUBLE...\n");
  printBoard(msBoard());
  printf("ACCETTAZIONE: %s\n", acceptDouble()?"OK":"NO");

  ms.fMove = 1;
  ms.fTurn = 0;
  ms.fCubeOwner = 1;
  SwapSides(ms.anBoard);
  printBoard(msBoard());
  printf("ACCETTAZIONE: %s\n", acceptDouble()?"OK":"NO");
  printf("AIlevel: %d\n\n", currentAILevel);
}


void testPlayTurn() {
  int b[2][25] = 
  {
    //MEGLIO NON RADDOPPIARE
    //{0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //PC

    //RICHIESTA DI RADDOPPIO SU 0 A 0
    //{1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0}, 
    //{0, 2, 2, 3, 0, 3, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //PC

    //RACE GAME..
    {0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
    {0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} //PC
  };
  
  setBoard((ConstTanBoard)b);
  ms.nCube = 1;
  ms.fCubeOwner = 0;
  ms.fMove = 0;
  ms.fTurn = 0;
  ms.anScore[0] = 4;
  ms.anScore[1] = 6;
  ms.nMatchTo = 7;
  ms.fCrawford = FALSE;

  printf("\n\nTEST TURNO IA...\n");
  printBoard((ConstTanBoard)b);
  printf("RESIGN: %d\n", askForResignation());
  printf("DOUBLING: %s\n", askForDoubling()?"YES":"NO");
  int dices[2] = {6, 3};
  int move[8];
  rollDice(dices);
  printDices(dices);
  evaluateBestMove(dices, move);
  printMove(move);
  printf("AIlevel: %d\n\n", currentAILevel);
}

void testGenerateMoves() {
  int _b[2][25] =   
  {
    {1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0}, 
    {0, 2, 2, 3, 0, 3, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //PC
  };
  
  ConstTanBoard b=ms.anBoard;
  printf("\n\nTEST GENERATE MOVES...\n");
  printBoard((ConstTanBoard)b);
  int dices[2] = {4, 6};
  printDices(dices);
  
  int** moves; 
  int nMoves;

  moves = generateMoves((ConstTanBoard)b, dices[0], dices[1], &nMoves);
  int i=0;
  for (i=0;i<nMoves;i++) {
    printMove(moves[i]);
  }
  printf("MOVES: %d\n", nMoves);
  printf("AIlevel: %d\n\n", currentAILevel);
}


void testAll () {
  initEnvironment("./");
  setAILevel(GRANDMASTER);
  testResignation();
  testDoubling();
  testPlayTurn();
  //testGenerateMoves();
}


int main (int argc, char** argv) {
  testAll();
}
