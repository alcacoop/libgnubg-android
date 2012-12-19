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


#include "imported_functions.h"
#include "globals.h"

extern void ProcessEvents(void)
{
  {
    while (g_main_context_pending(NULL))
      g_main_context_iteration(NULL, TRUE);
  }
}

extern void get_eq_before_resign(cubeinfo *pci, decisionData *pdd)
{
  //const evalcontext ecResign = { FALSE, 2, FALSE, TRUE, 0.0 };
  const evalcontext ecResign = { FALSE, 0, TRUE, TRUE, 0.0 };

  pdd->pboard = (ConstTanBoard)ms.anBoard;
  pdd->pci = pci;
  pdd->pec = &ecResign;

  if (ms.anDice[0] > 0) {
    float t;
    /* Opponent has rolled the dice and then resigned. We want to find out if the resignation is OK after
       the roll */
    RunAsyncProcess((AsyncFun) asyncEvalRoll, pdd, "Considering resignation...");
    /* Swap the equities as evaluation is for other player */
    pdd->aarOutput[0][OUTPUT_WIN] = 1 - pdd->aarOutput[0][OUTPUT_WIN];
    t = pdd->aarOutput[0][OUTPUT_WINGAMMON];
    pdd->aarOutput[0][OUTPUT_WINGAMMON] = pdd->aarOutput[0][OUTPUT_LOSEGAMMON];
    pdd->aarOutput[0][OUTPUT_LOSEGAMMON] = t;
    t = pdd->aarOutput[0][OUTPUT_WINBACKGAMMON];
    pdd->aarOutput[0][OUTPUT_WINBACKGAMMON] = pdd->aarOutput[0][OUTPUT_LOSEBACKGAMMON];
    pdd->aarOutput[0][OUTPUT_LOSEBACKGAMMON] = t;
  } else {
    RunAsyncProcess((AsyncFun) asyncMoveDecisionE, pdd, "Considering resignation...");
  }
}

extern void EvaluateRoll ( float ar[ NUM_ROLLOUT_OUTPUTS ], int nDie1, int nDie2, const TanBoard anBoard, 
    const cubeinfo *pci, const evalcontext *pec) {
  TanBoard anBoardTemp;
  cubeinfo ciOpp;

  memcpy ( &ciOpp, pci, sizeof ( cubeinfo ) );
  ciOpp.fMove = ! pci->fMove;

  memcpy( &anBoardTemp[ 0 ][ 0 ], &anBoard[ 0 ][ 0 ], 2 * 25 * sizeof( int ) );

  if( FindBestMove( NULL, nDie1, nDie2, anBoardTemp,
        (cubeinfo *) pci, NULL, defaultFilters ) < 0 )
    return;


  SwapSides( anBoardTemp );

  if ( GeneralEvaluationE( ar, (ConstTanBoard)anBoardTemp, &ciOpp, (evalcontext *) pec ) )
    return;

}

extern void init_rng(void)
{
  if (!rngctxCurrent) {
    rngctxCurrent = InitRNG(NULL, NULL, TRUE, rngCurrent);
    rngctxRollout = InitRNG(&rcRollout.nSeed, NULL, TRUE, rcRollout.rngRollout);
    rcRollout.nSeed ^= 0x792A584B;
  }
}


extern void GetMatchStateCubeInfo( cubeinfo* pci, const matchstate* pms )
{
  SetCubeInfo( pci, pms->nCube, pms->fCubeOwner, pms->fMove,
      pms->nMatchTo, pms->anScore, pms->fCrawford,
      pms->fJacoby, 3, pms->bgv );
}

extern int check_resigns(cubeinfo * pci)
{
  float rEqBefore, rEqAfter;
  const float max_cost = 0.05f;
  const float max_gain = 1e-6f;
  decisionData dd;
  cubeinfo ci;
  int resigned = 1;

  if (pci == NULL)
    {
      GetMatchStateCubeInfo(&ci, &ms);
      pci = &ci;
    }

  get_eq_before_resign(pci, &dd);
  do
    {
      getResignEquities(dd.aarOutput[0], pci, resigned, &rEqBefore, &rEqAfter);
      if (rEqBefore - rEqAfter > max_cost)
        {
          resigned=4;
          break;
        }
      else if (rEqAfter - rEqBefore < max_gain )
        break;
    }
  while (resigned++ < 3);
  return resigned == 4 ? -1 : resigned;
}

