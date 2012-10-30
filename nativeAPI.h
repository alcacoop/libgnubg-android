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
extern int acceptResign();

//API: EVALUATE WHETHER TO ACCEPT A UMAN BOUBLING REQUEST 
extern int acceptDouble();

//API: EVALUATE WHETHER TO RESIGN
extern int askForResignation();

//API: EVALUATE WHETHER TO ASK FOR A DOUBLE
extern int askForDoubling();

//API: EVALUATE THE BEST MOVE
extern void evaluateBestMove(int dices[2], int move[8]);

#endif
