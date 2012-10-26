#ifndef GLOBALS_H
#define GLOBALS_H

#include "dice.h"
#include "eval.h"
#include "backgammon.h"
#include "rollout.h"



#define TRUE 1
#define FALSE 0
#define EXIT_FAILURE -1

extern int fOutputMWC = FALSE;
extern int fShowProgress = FALSE;
extern int fAutoSaveRollout = FALSE;
extern int fAutoCrawford = FALSE;
extern rngcontext *rngctxRollout = NULL;


/* this is the "normal" movefilter*/
#define MOVEFILTER \
  { { { 0,  8, 0.16f }, {  0, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, {  0, 0, 0 } }, \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, { -1, 0, 0 } } , \
  }


extern rolloutcontext rcRollout =
{ 
  {
    /* player 0/1 cube decision */
    { TRUE, 0, TRUE, TRUE, 0.0 },
    { TRUE, 0, TRUE, TRUE, 0.0 }
  }, 
  {
    /* player 0/1 chequerplay */
    { TRUE, 0, TRUE, TRUE, 0.0 },
    { TRUE, 0, TRUE, TRUE, 0.0 }
  }, 

  {
    /* player 0/1 late cube decision */
    { TRUE, 0, TRUE, TRUE, 0.0 },
    { TRUE, 0, TRUE, TRUE, 0.0 }
  }, 
  {
    /* player 0/1 late chequerplay */
    { TRUE, 0, TRUE, TRUE, 0.0 },
    { TRUE, 0, TRUE, TRUE, 0.0 } 
  }, 
  /* truncation point cube and chequerplay */
  { TRUE, 0, TRUE, TRUE, 0.0 },
  { TRUE, 0, TRUE, TRUE, 0.0 },

  /* move filters */
  { MOVEFILTER, MOVEFILTER },
  { MOVEFILTER, MOVEFILTER },

  TRUE, /* cubeful */
  TRUE, /* variance reduction */
  FALSE, /* initial position */
  TRUE, /* rotate */
  TRUE, /* truncate at BEAROFF2 for cubeless rollouts */
  TRUE, /* truncate at BEAROFF2_OS for cubeless rollouts */
  FALSE, /* late evaluations */
  FALSE,  /* Truncation enabled */
  FALSE,  /* no stop on STD */
  FALSE,  /* no stop on JSD */
  FALSE,  /* no move stop on JSD */
  10, /* truncation */
  1296, /* number of trials */
  5,  /* late evals start here */
  RNG_MERSENNE, /* RNG */
  0,  /* seed */
  144,    /* minimum games  */
  0.01,	  /* stop when std's are lower than 0.01 */
  144,    /* minimum games  */
  1.96,   /* stop when best has j.s.d. for 95% confidence */
  0,      /* nGamesDone */
  0,      /* nSkip */
};


/*
int board [2][25] = 
{ 
  {0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
  {0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} 
  };

int board [2][25] = { 
  {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
  {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}
};
*/
extern matchstate ms = {
    {
      {0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
      {0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} 
    }, /* anBoard */
    {5,4}, /* anDice */
    0, /* fTurn */
    0, /* fResigned */
    0, /* fResignationDeclined */
    FALSE, /* fDoubled */
    0, /* cGames */
    -1, /* fMove */
    -1, /* fCubeOwner */
    FALSE, /* fCrawford */
    FALSE, /* fPostCrawford */
    7, /* nMatchTo */
    { 0, 0 }, /* anScore */
    1, /* nCube */
    0, /* cBeavers */
    VARIATION_STANDARD, /*bgv */
    TRUE, /* fCubeUse */
    TRUE, /* fJacoby */
    GAME_PLAYING
};



/*
cubeinfo ci = {
  .nCube = 1,
  .fCubeOwner = 1,
  .fMove = 0,
  .nMatchTo = 7,
  .anScore[0] = 0,
  .anScore[1] = 0,
  .fCrawford = 0,
  .fJacoby = 1,
  .fBeavers = 0,
  .bgv = VARIATION_STANDARD
};
*/

//CONFIGURAZIONE SUPREMO
extern evalcontext ec = {
  .fCubeful = 1,
  .nPlies = 1,
  .fUsePrune = 1,
  .fDeterministic = 1,
  .rNoise = 0.000
};

extern movefilter mf[] = {
  {0, 16, 0.32000},
  {0, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 16, 0.32000},
  {-1, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 0, 0.00000},
  {0, 16, 0.32000},
  {-1, 0, 0.00000},
  {0, 4, 0.08000},
  {0, 0, 0.00000},
  {0, 16, 0.32000},
  {-1, 0, 0.00000},
  {0, 4, 0.08000},
  {-1, 0, 0.00000}
};


#endif
