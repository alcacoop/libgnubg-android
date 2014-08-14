 /*
 ##################################################################
 #                     GNU BACKGAMMON MOBILE                      #
 ##################################################################
 #                                                                #
 #  Authors: Domenico Martella - Davide Saurino                   #
 #  E-mail: info@alcacoop.it                                      #
 #  Date:   19/12/2012                                            #
 #                                                                #
 ##################################################################
 #                                                                #
 #  Copyright (C) 2012   Alca Societa' Cooperativa                #
 #                                                                #
 #  This file is part of GNU BACKGAMMON MOBILE.                   #
 #  GNU BACKGAMMON MOBILE is free software: you can redistribute  # 
 #  it and/or modify it under the terms of the GNU General        #
 #  Public License as published by the Free Software Foundation,  #
 #  either version 3 of the License, or (at your option)          #
 #  any later version.                                            #
 #                                                                #
 #  GNU BACKGAMMON MOBILE is distributed in the hope that it      #
 #  will be useful, but WITHOUT ANY WARRANTY; without even the    #
 #  implied warranty of MERCHANTABILITY or FITNESS FOR A          #
 #  PARTICULAR PURPOSE.  See the GNU General Public License       #
 #  for more details.                                             #
 #                                                                #
 #  You should have received a copy of the GNU General            #
 #  Public License v3 along with this program.                    #
 #  If not, see <http://http://www.gnu.org/licenses/>             #
 #                                                                #
 ##################################################################
*/


#include "nativeAPI.h"
#include "util.h"

pthread_t thread;


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
  printf("\n\nTEST ACCETTAZIONE DOUBLE...\n");
  int b[2][25] = 
  {
    {2, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},//ALL BEARED OFF 
    {0, 3, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0},//ALL BEARED OFF
    //{2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0}
  };

  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;


  ms.fMove = 0;
  ms.fTurn = 1;
  ms.fCubeOwner = -1;
  setBoard((ConstTanBoard)b);
  printBoard(msBoard());
  printf("ACCETTAZIONE: %s\n\n", acceptDouble()?"OK":"NO");

  ms.fMove = 1;
  ms.fTurn = 0;
  ms.fCubeOwner = -1;
  SwapSides(ms.anBoard);
  printBoard(msBoard());
  printf("ACCETTAZIONE: %s\n", acceptDouble()?"OK":"NO");
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
    //{0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
    //{0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} //PC
    
    {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},//BLACK (HUMAN) BGV 
    {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}//WHITE (PC)
  };
  
  setBoard((ConstTanBoard)b);
  ms.nCube = 1;
  ms.fCubeOwner = 1;
  ms.fMove = 1;
  ms.fTurn = 1;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 1;
  ms.fCrawford = FALSE;

  printf("\n\nTEST TURNO IA...\n");
  printBoard((ConstTanBoard)b);
  //printf("RESIGN: %d\n", askForResignation());
  //printf("DOUBLING: %s\n", askForDoubling()?"YES":"NO");
  int dices[2] = {6, 4};
  int move[8];
  //rollDice(dices);
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
  
  ConstTanBoard b=(ConstTanBoard)ms.anBoard;
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

void testGreedyBearoff() {
  int _b[2][25] =   
  {
    {0, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 2, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} //PC
  };
  
  setBoard((ConstTanBoard)_b);
  ConstTanBoard b=(ConstTanBoard)ms.anBoard;
  printf("\n\nTEST GREEDY BEAROFF...\n");
  printBoard((ConstTanBoard)b);
  int dices[2] = {6, 4};
  printDices(dices);
  
  TryBearoff(b, dices[0], dices[1]);
}


void testDiceGenerators() {
  int i=0;
  printf("\n\nISAAC SEQUENCE...\n");
  initRNG(1);
  for (i=0;i<10;i++){
    int d[2];
    rollDice(d);
    printDices(d);
  }

  printf("\n\nMERSENNE-TWISTER SEQUENCE...\n");
  initRNG(0);
  for (i=0;i<10;i++){
    int d[2];
    rollDice(d);
    printDices(d);
  }
  printf("\n");
}


void* testAll (void* o) {
  initEnvironment("./");
  initRNG(0);
  setAILevel(GRANDMASTER);
  testPlayTurn();
  testResignation();
  testDoubling();
  testGenerateMoves();
  testGreedyBearoff();
  testDiceGenerators();
}


int main (int argc, char** argv) {
  testAll(NULL);

  /* InitRNGSeed(893358218, RNG_ISAAC, rngctxCurrent); */
  /* InitRNGSeed(893358218, RNG_MERSENNE, rngctxCurrent); */
  /* RNGSystemSeed(RNG_ISAAC, rngctxCurrent, NULL); */
  /* pthread_create(&thread, NULL, testAll, NULL); */
  /* pthread_join(thread, NULL); */
}
