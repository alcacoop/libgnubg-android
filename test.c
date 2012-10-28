#include "globals.h"
#include "async.h"
#include "imported_functions.h"

extern evalcontext ec;
extern movefilter mf[];
extern matchstate ms;


void printBoard(int board [2][25]) {
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

void printMove(int move[8]){
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

void findMove(int d1, int d2) {
  int move [8];
  cubeinfo ci;
  int *board = ms.anBoard;

  GetMatchStateCubeInfo(&ci, &ms);
  FindBestMove(move, d1, d2, board, &ci, &ec, mf);
  printMove(move);
};

void rollDice(int d[2]) {
  unsigned long a;
  int b;
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  
  RollDice(d, &_rng, rngctxCurrent); 
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


//TODO: move to API
int acceptResign() {
  int *board = ms.anBoard;
  cubeinfo ci;
  GetMatchStateCubeInfo(&ci, &ms);
  
  int resign;
  if (ms.fResigned == -1) {
    MYLOG("RESIGNED = -1\n");
    resign = check_resigns(&ci);
  }
  else {
    float rEqBefore, rEqAfter;
    const float max_gain = 1e-6f;
    decisionData dd;
    
    get_eq_before_resign(&ci, &dd);
    getResignEquities(dd.aarOutput[0], &ci, ms.fResigned, &rEqBefore, &rEqAfter);
    if (rEqAfter - rEqBefore < max_gain )
      resign = ms.fResigned;
    else
      resign = -1;
  }

  char buf[50];
  if (resign > 0) {
    ms.fResigned = resign;
    sprintf(buf, "RESIGN WITH VALUE %d ACCEPTED!\n", ms.fResigned);
  }
  else {
    sprintf(buf, "RESIGN WITH VALUE %d NOT ACCEPTED!\n", ms.fResigned);
  }
  MYLOG(buf);

}



//TODO: move to API
void acceptDouble() {
  decisionData dd;
  cubedecision cd;
  cubeinfo ci;
  float arDouble[4];
  evalsetup es;
  es.et = EVAL_EVAL;
  es.ec = ec;
  es.rc = rcRollout;

  GetMatchStateCubeInfo(&ci, &ms);

  /* Evaluate cube decision */
  dd.pboard = ms.anBoard;
  dd.pci = &ci;
  dd.pes = &es;
  if (RunAsyncProcess((AsyncFun)asyncCubeDecision, &dd, "Considering cube action...") != 0)
    return -1;
  cd = FindCubeDecision(arDouble, dd.aarOutput, &ci);

  /* normal double by opponent */
  switch (cd) {
    case DOUBLE_TAKE:
    case NODOUBLE_TAKE:
    case TOOGOOD_TAKE:
    case REDOUBLE_TAKE:
    case NO_REDOUBLE_TAKE:
    case TOOGOODRE_TAKE:
    case NODOUBLE_DEADCUBE:
    case NO_REDOUBLE_DEADCUBE:
    case OPTIONAL_DOUBLE_TAKE:
    case OPTIONAL_REDOUBLE_TAKE:
      MYLOG("DOUBLE ACCEPTED!\n");
      break;

    case DOUBLE_PASS:
    case TOOGOOD_PASS:
    case REDOUBLE_PASS:
    case TOOGOODRE_PASS:
    case OPTIONAL_DOUBLE_PASS:
    case OPTIONAL_REDOUBLE_PASS:
      MYLOG("DOUBLE NOT ACCEPTED!\n");
      break;

    case DOUBLE_BEAVER:
    case NODOUBLE_BEAVER:
    case NO_REDOUBLE_BEAVER:
    case OPTIONAL_DOUBLE_BEAVER:
      MYLOG("DOUBLE ACCEPTED!\n");
      break;

    default:
      g_assert(FALSE);
  } 
}


int playTurn(){

  findData fd;
  TanBoard anBoardMove;
  float arResign[NUM_ROLLOUT_OUTPUTS];
  cubeinfo ci;
  float arDouble[4];
  float rDoublePoint;
  char buf[100];

  GetMatchStateCubeInfo(&ci, &ms);
  memcpy(anBoardMove, ms.anBoard, sizeof(TanBoard));

  /* Consider resigning */
  int nResign;
  evalcontext ecResign = {FALSE, 0, FALSE, TRUE, 0.0};
  evalsetup esResign;
  esResign.et = EVAL_EVAL;
  esResign.ec = ecResign;
  nResign = getResignation(arResign, ms.anBoard, &ci, &esResign);
  if (nResign > 0) {
    sprintf(buf, "RESIGN %d!\n", nResign);
    return 0;
  } else {
    sprintf(buf, "NOT RESIGN %d!\n", nResign);
  }
  MYLOG(buf);


  /* Consider doubling */
  //if (ms.fCubeUse && ms.nCube < MAX_CUBE && GetDPEq(NULL, NULL, &ci)) {
  if (ms.fCubeUse && ms.nCube < MAX_CUBE && GetDPEq(NULL, NULL, &ci)) {
  //if (TRUE) {
    evalcontext ecDH;
    float arOutput[NUM_ROLLOUT_OUTPUTS];
    memcpy(&ecDH, &ec, sizeof ecDH);
    ecDH.fCubeful = FALSE;
    if (ecDH.nPlies) ecDH.nPlies--;

    /* We have access to the cube */
    /* Determine market window */
    if (EvaluatePosition(NULL, (ConstTanBoard)anBoardMove, arOutput, &ci, &ecDH))
      return -1;

    rDoublePoint = GetDoublePointDeadCube(arOutput, &ci);
    if (arOutput[0] >= rDoublePoint) {
      /* We're in market window */
      evalsetup es;
      es.et = EVAL_EVAL;
      es.ec = ec;
      es.rc = rcRollout;
      decisionData dd;
      cubedecision cd;

      /* Consider cube action */
      dd.pboard = ms.anBoard;
      dd.pci = &ci;
      dd.pes = &es;
      if (RunAsyncProcess((AsyncFun)asyncCubeDecision, &dd, "Considering cube action...") != 0)
        return -1;

      cd = FindCubeDecision(arDouble, dd.aarOutput, &ci);
      switch (cd) {
        case DOUBLE_TAKE:
        case REDOUBLE_TAKE:
        case DOUBLE_PASS:
        case REDOUBLE_PASS:
        case DOUBLE_BEAVER:
          MYLOG("RICHIESTA DI RADDOPPIO!\n");
          return 0;

        case NODOUBLE_TAKE:
        case TOOGOOD_TAKE:
        case NO_REDOUBLE_TAKE:
        case TOOGOODRE_TAKE:
        case TOOGOOD_PASS:
        case TOOGOODRE_PASS:
        case NODOUBLE_BEAVER:
        case NO_REDOUBLE_BEAVER:
          MYLOG("MEGLIO NON RADDOPPIARE!\n");
          break;

        case OPTIONAL_DOUBLE_BEAVER:
        case OPTIONAL_DOUBLE_TAKE:
        case OPTIONAL_REDOUBLE_TAKE:
        case OPTIONAL_DOUBLE_PASS:
        case OPTIONAL_REDOUBLE_PASS:
          if (ec.nPlies==0) { /* double if 0-ply */
            MYLOG("RICHIESTA DI RADDOPPIO (0-ply)!\n");
            return 0;
          }
          MYLOG("MEGLIO NON RADDOPPIARE (0-ply)!\n");
          break;
        default:
          MYLOG("ASSERT FALSE!\n");
          break;
      }
    } /* market window */
  } /* access to cube */


  /* Roll dice and move */
  RollDice(ms.anDice, &rngCurrent, rngctxCurrent);
  sprintf(buf, "DICE: %d %d\n", ms.anDice[0], ms.anDice[1]);
  MYLOG(buf);
  findMove(ms.anDice[0], ms.anDice[1]);

  return 0;
}


void testResignation() {
  unsigned int b[2][25] = 
  {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
  };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  ms.fResigned = 3;
  // fTurn = chi deve decidere (accettare double/resign: 0: umano; 1: pc)
  // fMove = chi deve lanciare i dadi (0: umano; 1: pc)
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  MYLOG("TEST ACCETTAZIONE RESIGN...\n");
  printBoard(ms.anBoard);
  acceptResign();
  MYLOG("\n\n");
}



void testDoubling() {
  unsigned int b[2][25] = 
  {
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15}
  };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  ms.fDoubled = 1;
  ms.nCube = 4;
  ms.fCubeOwner = -1;
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  MYLOG("TEST ACCETTAZIONE DOUBLE...\n");
  printBoard(ms.anBoard);
  acceptDouble();
  MYLOG("\n\n");
}



void testPlayTurn() {
  unsigned int b[2][25] = 
  {
    //MEGLIO NON RADDOPPIARE
    //{0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    //RICHIESTA DI RADDOPPIO
    //{0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    //RACE GAME..
    {0, 0, 0, 0, 2, 5, 2, 2, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0}, 
    {0, 0, 0, 2, 0, 4, 2, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0} 
  };
  memcpy(ms.anBoard, b, sizeof(TanBoard));
  ms.nCube = 1;
  ms.fCubeOwner = 0;
  ms.fMove = 0;
  ms.fTurn = 0;
  ms.anScore[0] = 3;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  MYLOG("TEST TURNO IA...\n");
  printBoard(ms.anBoard);
  playTurn();
  MYLOG("\n\n");
}




int main (int argc, char** argv) {
  init_rng();
  EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  InitMatchEquity("zadeh.xml");
  MYLOG("\n");

  testResignation();
  testDoubling();
  testPlayTurn();
}
