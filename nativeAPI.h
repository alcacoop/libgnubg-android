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


#ifndef NATIVE_API_H
#define NATIVE_API_H

#include "globals.h"
#include "async.h"
#include "levels.h"
#include "imported_functions.h"

//API: ROLL DICES WITH MERSENNE TWISTER ALGORITHM
void rollDice(int dices[2]);

//API: INITIALIZE ENV (NET EVALUATOR AND EQUITIES TABLE)
void initEnvironment(const char*);

//API: SET NET AI LEVEL
void setAILevel(available_levels l);

//API: EVALUATE WHETHER TO ACCEPT A UMAN RESIGN 
//params: int r = resign value request
//return: 0 => resign not accepted
//       >0 => resign accepted
int acceptResign(int r);

//API: UPDATE CUBEINFO IN GLOBAL MATCH STATE VAR
//params: int nCube = new cube value
//        int nCube = new cube owner
void updateMSCubeInfo(int nCube, int fCubeOwner);

//API: EVALUATE WHETHER TO ACCEPT A UMAN DOUBLING REQUEST 
//return: 0 => double not accepted
//        1 => double accepted
int acceptDouble();

//API: EVALUATE WHETHER TO RESIGN
//return: 0 => keep playing, do not resign
//        >0 => I'm asking for a resign with this value
int askForResignation();

//API: EVALUATE WHETHER TO ASK FOR A DOUBLE
//return: 0 => better don't doubling
//        1 => ask for doubling
//       -1 => error
int askForDoubling();

//API: EVALUATE THE BEST MOVE
void evaluateBestMove(int dices[2], int move[8]);

//API: SET BOARD
void setBoard(ConstTanBoard b);

//API: SET MATCH SCORE
//params: int AIScore = AI score
//        int HumanSclore = HumanScore
void setMatchScore(int AIScore, int HumanScore);

//API: SET GAME TURN
//params: int fTurn = decision turn (0=AI, 1=Human)
//        int fMove = roll turn (0=AI, 1=Human)
void setGameTurn(int fTurn, int fMove);


//API: SET MATCHTO SCORE
//params: int matchTo = ms.nMatchTo
void setMatchTo(int matchTo);


//API: GENERATE ALL AVAILABLE MOVES
//params: int matchTo = ms.nMatchTo
int** generateMoves(ConstTanBoard, int, int, int*); 

//API: SET GAME VARIANT (I.E. BACKGAMMON OR NACKGAMMON)
//params: match type
void setGameVariant(int type);

//API: SET IF WE USE CUBE OR NOT IN THE MATCH
void setCubeUse(int fCubeUse);

//API: SET IF WE USE THE CRAWFORD RULE
void setCrawford(int fCrawford);

void setPostCrawford(int fPostCrawford);

#endif
