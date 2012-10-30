#include "nativeAPI.h"

//UTILS: PRINT A BOARD
void printBoard(ConstTanBoard board) {
  int i, j;
  char buf[200];
  sprintf(buf, "");
  for(i = 0; i<2; i++) {
    for(j=0; j<25; j++) {
      sprintf(buf+strlen(buf), " %d ", board[i][j]);
    }
    sprintf(buf+strlen(buf), "\n");
  }
  MYLOG(buf);
}


//UTILS: PRINT A MOVE
void printMove(int move[8]) {
  int i=0;
  char buf[200];
  sprintf(buf, "MOVE: ");
  sprintf(buf+strlen(buf), "%d/%d ", move[0], move[1]);
  sprintf(buf+strlen(buf), "%d/%d ", move[2], move[3]);
  sprintf(buf+strlen(buf), " | %d/%d ", move[4], move[5]);
  sprintf(buf+strlen(buf), "%d/%d", move[6], move[7]);
  sprintf(buf+strlen(buf), "\n");
  MYLOG(buf);
}


//UTILS: PRINT A COUPLE OF DICES
void printDices(int dices[2]) {
  int i=0;
  char buf[200];
  sprintf(buf, "DICES: %d %d\n", dices[0], dices[1]);
  MYLOG(buf);
}

void testResignation() {
  unsigned int b[2][25] = 
  {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //PC
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
  };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  //  ms.fResigned = 3;
  // fTurn = chi deve decidere (accettare double/resign: 0: umano; 1: pc)
  // fMove = chi deve lanciare i dadi (0: umano; 1: pc)
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  MYLOG("TEST ACCETTAZIONE RESIGN...\n");
  printBoard(msBoard());
  acceptResign(1);
  acceptResign(2);
  acceptResign(3);
  MYLOG("\n\n");
}



void testDoubling() {
  unsigned int b[2][25] = 
  {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //PC
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
  };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  //  ms.fDoubled = 1;
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  MYLOG("TEST ACCETTAZIONE DOUBLE...\n");
  printBoard(msBoard());
  printf("RITORNO: %d\n", acceptDouble());
  MYLOG("\n\n");
}


void testPlayTurn() {
  TanBoard b = 
  //  unsigned int b[2][25] = 
  {
    //MEGLIO NON RADDOPPIARE
    //{0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //PC

    //RICHIESTA DI RADDOPPIO SU 0 A 0
    //{1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0}, 
    //{0, 2, 2, 3, 0, 3, 2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //PC

    //RACE GAME..
    //{0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
    //{0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} //PC

    //INIT GAME
    //{0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
    //{0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0} //PC

    //INIT GAME
    {0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0} //PC
  };
  TanBoard c = BACKGAMMON_BOARD;
  setBoard(&b);
  ms.nCube = 1;
  ms.fCubeOwner = 0;
  ms.fMove = 0;
  ms.fTurn = 0;
  ms.anScore[0] = 4;
  ms.anScore[1] = 6;
  ms.nMatchTo = 7;
  ms.fCrawford = FALSE;

  MYLOG("TEST TURNO IA...\n");
  printf("RESULT ASKDOUBLING: %d\n", askForDoubling());
  printf("RESULT ASKRESIGNATION: %d\n", askForResignation());
  int dices[2] = {6, 3};
  int move[8];
  rollDice(dices);
  printDices(dices);
  evaluateBestMove(dices, move);
  printMove(move);
  MYLOG("\n\n");
}


void testAll () {
  initEnvironment();
  setAILevel(EXPERT);
  // testResignation();
  // testDoubling();
  testPlayTurn();
}


int main (int argc, char** argv) {
  testAll();
}
