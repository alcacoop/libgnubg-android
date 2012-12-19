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

  setAILevel(SUPREMO);

  //setAILevel(GRANDMASTER);
  //testPlayTurn();
  //testResignation();
  testDoubling();
  //testGenerateMoves();
}


int main (int argc, char** argv) {
  testAll();
}
