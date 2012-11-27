#include "nativeAPI.h"

char* DATA_DIR;
int currentAILevel;

void rollDice(int dices[2]) {
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  RollDice(dices, &_rng, rngctxCurrent); 
}


void initEnvironment(const char* path) {
  size_t len = strlen(path);
  DATA_DIR = (char*)calloc(len, sizeof(char));
  sprintf(DATA_DIR, "%s", path);

  char *w1, *w2, *met;
  w1 = BuildFilename("gnubg.weights");
  w2 = BuildFilename("gnubg.wd");
  met = BuildFilename("g11.xml");
  EvalInitialise(w1, w2, 0, NULL);
  InitMatchEquity(met);
  init_rng();
}


void setAILevel(available_levels l) {
  currentAILevel = l;
  memcpy(&ec, &levels[l].ec, sizeof(evalcontext));
  memcpy(&mf, &levels[l].mf, sizeof(movefilter)*16);
}


int acceptResign(int r) {
  ConstTanBoard board = msBoard();
  cubeinfo ci;
  ms.fResigned = r;
  GetMatchStateCubeInfo(&ci, &ms);
  
  int resign = 0;
  float rEqBefore, rEqAfter;
  const float max_gain = 1e-6f;
  decisionData dd;
  
  get_eq_before_resign(&ci, &dd);
  getResignEquities(dd.aarOutput[0], &ci, ms.fResigned, &rEqBefore, &rEqAfter);
  if (rEqAfter - rEqBefore < max_gain )
    resign = ms.fResigned;

  return resign;
}


void updateMSCubeInfo(int nCube, int fCubeOwner) {
  ms.nCube = nCube;
  ms.fCubeOwner = fCubeOwner;
}


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
      updateMSCubeInfo(ms.nCube * 2, 0); //accept double
      return 1;
      break;

    case DOUBLE_PASS:
    case TOOGOOD_PASS:
    case REDOUBLE_PASS:
    case TOOGOODRE_PASS:
    case OPTIONAL_DOUBLE_PASS:
    case OPTIONAL_REDOUBLE_PASS:
      ms.anScore[1] = ms.nCube;
      updateMSCubeInfo(1, -1); // reset cube values
      return 0;
      break;

    case DOUBLE_BEAVER:
    case NODOUBLE_BEAVER:
    case NO_REDOUBLE_BEAVER:
    case OPTIONAL_DOUBLE_BEAVER:
      updateMSCubeInfo(ms.nCube * 2, 0); //accept double
      return 1;
      break;

    default:
      g_assert(FALSE);
  } 
}


int askForResignation() {
  TanBoard anBoardMove;
  cubeinfo ci;
  float arResign[NUM_ROLLOUT_OUTPUTS];

  GetMatchStateCubeInfo(&ci, &ms);
  memcpy(anBoardMove, msBoard(), sizeof(TanBoard));
  SwapSides(anBoardMove);

  /* Consider resigning */
  int nResign;
  evalcontext ecResign = {FALSE, 0, FALSE, TRUE, 0.0};
  evalsetup esResign;
  esResign.et = EVAL_EVAL;
  esResign.ec = ecResign;
  nResign = getResignation(arResign, anBoardMove, &ci, &esResign);

  if (nResign > 0) {
    return nResign;
  }
  return 0;
}


int askForDoubling() {
  TanBoard anBoardMove;
  cubeinfo ci;
  float arDouble[4];
  float rDoublePoint;
  char buf[100];

  GetMatchStateCubeInfo(&ci, &ms);
  memcpy(anBoardMove, ms.anBoard, sizeof(TanBoard));
  SwapSides(anBoardMove);

  if ((ms.fCubeOwner == -1) || (ms.fCubeOwner == 0)) {
    /* Consider doubling */
    if (ms.fCubeUse && ms.nCube < MAX_CUBE && GetDPEq(NULL, NULL, &ci)) {
      evalcontext ecDH;
      if (currentAILevel<5)
        memcpy(&ecDH, &ec, sizeof(ecDH));
      else
        memcpy(&ecDH, &levels[5].ec, sizeof(evalcontext));

      float arOutput[NUM_ROLLOUT_OUTPUTS];
      ecDH.fCubeful = FALSE;
      if (ecDH.nPlies) ecDH.nPlies--;

      /* Determine market window */
      if (EvaluatePosition(NULL, (ConstTanBoard)anBoardMove, arOutput, &ci, &ecDH)) {
        return -1; //ERROR
      }

      float f;
      rDoublePoint = GetDoublePointDeadCube(arOutput, &ci, &f);

      if (arOutput[0] >= f) {
        /* We're in market window */
        evalsetup es;
        es.et = EVAL_EVAL;
        es.ec = ecDH;
        es.rc = rcRollout;
        decisionData dd;
        cubedecision cd;

        /* Consider cube action */ 
        dd.pboard = (ConstTanBoard)anBoardMove;
        dd.pci = &ci;
        dd.pes = &es;
        if (RunAsyncProcess((AsyncFun)asyncCubeDecision, &dd, "Considering cube action...") != 0)
          return -1; // error

        cd = FindCubeDecision(arDouble, dd.aarOutput, &ci);
        switch (cd) {
        case DOUBLE_TAKE:
        case REDOUBLE_TAKE:
        case DOUBLE_PASS:
        case REDOUBLE_PASS:
        case DOUBLE_BEAVER:
          return 1;

        case NODOUBLE_TAKE:
        case TOOGOOD_TAKE:
        case NO_REDOUBLE_TAKE:
        case TOOGOODRE_TAKE:
        case TOOGOOD_PASS:
        case TOOGOODRE_PASS:
        case NODOUBLE_BEAVER:
        case NO_REDOUBLE_BEAVER:
          return 0;

        case OPTIONAL_DOUBLE_BEAVER:
        case OPTIONAL_DOUBLE_TAKE:
        case OPTIONAL_REDOUBLE_TAKE:
        case OPTIONAL_DOUBLE_PASS:
        case OPTIONAL_REDOUBLE_PASS:
          if (ec.nPlies==0) /* double if 0-ply */
            return 1;
          else 
            return 0;

        default:
          return -1; // error
        }
      } /* market window */
      else {
        return 0;
      }
    } /* access to cube */
  }
  return 0; //IA can't play the cube
}


void evaluateBestMove(int dices[2], int move[8]) {
  TanBoard anBoardMove;
  cubeinfo ci;

  GetMatchStateCubeInfo(&ci, &ms);
  memcpy(anBoardMove, ms.anBoard, sizeof(TanBoard));
  SwapSides(anBoardMove);
  FindBestMove(move, dices[0], dices[1], anBoardMove, &ci, &ec, mf);
}


void setBoard(ConstTanBoard b) {
  memcpy(ms.anBoard, b, sizeof(TanBoard));
}


void setMatchScore(int AIScore, int HumanScore) {
  ms.anScore[0] = AIScore;
  ms.anScore[1] = HumanScore;
}


void setGameTurn(int fTurn, int fMove) {
  ms.fTurn = fTurn;
  ms.fMove = fMove;
}


void setMatchTo(int matchTo) {
  ms.nMatchTo = matchTo;
}


int** generateMoves(ConstTanBoard b, int d1, int d2, int* l) {
  int f = 2;
  if (d1==d2) f=1;

  movelist ml;
  int i=0;
  int j=0;
  int pattern[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

  GenerateMoves(&ml, b, d1, d2, 0);
  if (ml.cMoves==0) {
    *l = 0;
    return NULL;
  }

  int** moves = (int **)malloc(f * ml.cMoves * sizeof(int *));

  for(i = 0; i < ml.cMoves; i++) {
    moves[i] = (int *)malloc(8 * sizeof(int));
    memcpy(moves[i], pattern, 8*sizeof(int));
    move m = ml.amMoves[i];
    for (j=0;j<4;j++) {
      if (m.anMove[2*j]==-1) break;
      moves[i][2*j] = m.anMove[2*j];
      moves[i][2*j+1] = m.anMove[2*j+1];
    }
  }



  if (f == 2) {
    GenerateMoves(&ml, b, d2, d1, 0);

    for(i = ml.cMoves; i <  2*ml.cMoves; i++) {
      moves[i] = (int *)malloc(8 * sizeof(int));
      memcpy(moves[i], pattern, 8*sizeof(int));
      move m = ml.amMoves[i-ml.cMoves];
      for (j=0;j<4;j++) {
        if (m.anMove[2*j]==-1) break;
        moves[i][2*j] = m.anMove[2*j];
        moves[i][2*j+1] = m.anMove[2*j+1];
      }
    }
  }

  *l = f * ml.cMoves;
  return moves;
}
