#include "nativeAPI.h"

//API: ROLL DICES WITH MERSENNE TWISTER ALGORITHM
void rollDice(int dices[2]) {
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  RollDice(dices, &_rng, rngctxCurrent); 
}


//API: INITIALIZE ENV (NET EVALUATOR AND EQUITIES TABLE)
void initEnvironment() {
  char *w1, *w2, *met;
  w1 = BuildFilename("gnubg.weights");
  w2 = BuildFilename("gnubg.wd");
  met = BuildFilename("g11.xml");
  EvalInitialise(w1, w2, 0, NULL);
  InitMatchEquity(met);
  init_rng();
}


//API: SET NET AI LEVEL
void setAILevel(available_levels l) {
  memcpy(&ec, &levels[l].ec, sizeof(evalcontext));
  memcpy(&mf, &levels[l].mf, sizeof(movefilter)*16);
}


//API: EVALUATE WHETHER TO ACCEPT A UMAN RESIGN 
int acceptResign() {
  ConstTanBoard board = msBoard();
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


//API: EVALUATE WHETHER TO ACCEPT A UMAN BOUBLING REQUEST 
int acceptDouble() {
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
  dd.pboard = msBoard();
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


//API: EVALUATE WHETHER TO RESIGN
int askForResignation() {
  TanBoard anBoardMove;
  cubeinfo ci;
  char buf[100];
  float arResign[NUM_ROLLOUT_OUTPUTS];

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
  return 0;
}


//API: EVALUATE WHETHER TO ASK FOR A DOUBLE
int askForDoubling() {
  TanBoard anBoardMove;
  cubeinfo ci;
  float arDouble[4];
  float rDoublePoint;
  char buf[100];

  GetMatchStateCubeInfo(&ci, &ms);
  memcpy(anBoardMove, ms.anBoard, sizeof(TanBoard));


  /* Consider doubling */
  if (ms.fCubeUse && ms.nCube < MAX_CUBE && GetDPEq(NULL, NULL, &ci)) {
  //if (WE HAVE ACCESS TO CUBE) {
    evalcontext ecDH;
    float arOutput[NUM_ROLLOUT_OUTPUTS];
    memcpy(&ecDH, &ec, sizeof ecDH);
    ecDH.fCubeful = FALSE;
    if (ecDH.nPlies) ecDH.nPlies--;

    /* Determine market window */
    if (EvaluatePosition(NULL, (ConstTanBoard)anBoardMove, arOutput, &ci, &ecDH)) {
      MYLOG("EVALUATE POSITION ERROR");
      return -1;
    }

    float f;
    rDoublePoint = GetDoublePointDeadCube(arOutput, &ci, &f);

    if (arOutput[0] >= f) {
      /* We're in market window */
      evalsetup es;
      es.et = EVAL_EVAL;
      es.ec = ec;
      es.rc = rcRollout;
      decisionData dd;
      cubedecision cd;

      /* Consider cube action */ 
      dd.pboard = msBoard();
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
    else {
      MYLOG("NOT IN MARKET WINDOW\n");
    }
  } /* access to cube */

}


//API: EVALUATE THE BEST MOVE
void evaluateBestMove(int dices[2], int move[8]) {
  TanBoard anBoardMove;
  cubeinfo ci;

  GetMatchStateCubeInfo(&ci, &ms);
  memcpy(anBoardMove, ms.anBoard, sizeof(TanBoard));
  FindBestMove(move, dices[0], dices[1], anBoardMove, &ci, &ec, mf);
}
