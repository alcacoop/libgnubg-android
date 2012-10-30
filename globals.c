#include "globals.h"
#include "dice.h"
#include "backgammon.h"
#include "rollout.h"


int fOutputMWC = FALSE;
int fShowProgress = FALSE;
int fAutoSaveRollout = FALSE;
int fAutoCrawford = FALSE;
rngcontext *rngctxRollout = NULL;

evalcontext ec;
movefilter mf[4][4];


/* this is the "normal" movefilter*/
#define MOVEFILTER \
  { { { 0,  8, 0.16f }, {  0, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 0, 0    }, {  0, 0, 0 } } , \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, {  0, 0, 0 } }, \
    { { 0,  8, 0.16f }, { -1, 0, 0 }, { 0, 2, 0.04f }, { -1, 0, 0 } } , \
  }


rolloutcontext rcRollout =
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



matchstate ms = {
    {
      {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0},
      {0, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0}
    }, /* anBoard */
    {0,0}, /* anDice */
    0, /* fTurn: CHI DEVE PRENDERE LA DECISIONE ATTUALE*/
    0, /* fResigned */
    0, /* fResignationDeclined */
    FALSE, /* fDoubled */
    0, /* cGames */
    -1, /* fMove: CHI HA I DADI IN MANO */
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


char* BuildFilename(char* str)
{
  char* buf;
  buf=(char*)calloc(255, sizeof(char));
  strcat(buf, DATA_DIR);
  strcat(buf, str);
  
  return buf;
}

ConstTanBoard msBoard(){return (ConstTanBoard)ms.anBoard;};

#ifdef IS_ANDROID
#include <android/log.h>
void MYLOG(char* s) {__android_log_write(ANDROID_LOG_ERROR,"MYLOG", s);};
#else
void MYLOG(char* s) {printf("%s", s);}
#endif
