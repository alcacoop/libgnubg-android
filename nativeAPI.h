#ifndef NATIVE_API_H
#define NATIVE_API_H

#include "globals.h"
#include "async.h"
#include "levels.h"
#include "imported_functions.h"

//API: ROLL DICES WITH MERSENNE TWISTER ALGORITHM
extern void rollDice(int dices[2]);

//API: INITIALIZE ENV (NET EVALUATOR AND EQUITIES TABLE)
extern void initEnvironment();

//API: SET NET AI LEVEL
extern void setAILevel(available_levels l);

//API: EVALUATE WHETHER TO ACCEPT A UMAN RESIGN 
//params: int r = resign value request
//return: 0 => resign not accepted
//       >0 => resign accepted
extern int acceptResign(unsigned int r);

//API: UPDATE CUBEINFO IN GLOBAL MATCH STATE VAR
//params: int nCube = new cube value
//        int nCube = new cube owner
void updateMSCubeInfo(int nCube, int fCubeOwner);

//API: EVALUATE WHETHER TO ACCEPT A UMAN DOUBLING REQUEST 
//return: 0 => double not accepted
//        1 => double accepted
extern int acceptDouble();

//API: EVALUATE WHETHER TO RESIGN
//return: 0 => keep playing, do not resign
//        >0 => I'm asking for a resign with this value
extern int askForResignation();

//API: EVALUATE WHETHER TO ASK FOR A DOUBLE
//return: 0 => better don't doubling
//        1 => ask for doubling
//       -1 => error
extern int askForDoubling();

//API: EVALUATE THE BEST MOVE
extern void evaluateBestMove(int dices[2], int move[8]);

//API: SET BOARD
void setBoard(TanBoard b);
#endif