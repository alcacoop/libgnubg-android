#include "jniAPI.h"
#include "globals.h"
#include "nativeAPI.h"



JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_RollDice
  (JNIEnv * env, jclass class, jintArray dices) 
{
  jint *ds = (*env)->GetIntArrayElements(env, dices, 0);
  rollDice(ds);
  (*env)->ReleaseIntArrayElements(env, dices, ds, 0);
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_InitializeEnvironment
  (JNIEnv *env, jclass class, jstring str)
{
  const char* s = (*env)->GetStringUTFChars(env, str, 0);
  initEnvironment(s);
  (*env)->ReleaseStringUTFChars(env, str, s);
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_SetAILevel
  (JNIEnv * env, jclass class, jint level)
{
  setAILevel(level);
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_SetBoard
  (JNIEnv * env, jclass class, jintArray _b1, jintArray _b2) 
{
  jint *b1 = (*env)->GetIntArrayElements(env, _b1, 0);
  jint *b2 = (*env)->GetIntArrayElements(env, _b2, 0);

  int b[2][25];
  memcpy(b[0], b1, sizeof(int)*25);
  memcpy(b[1], b2, sizeof(int)*25);
  setBoard((ConstTanBoard)b);

  (*env)->ReleaseIntArrayElements(env, _b1, b1, 0);
  (*env)->ReleaseIntArrayElements(env, _b2, b2, 0);
}


JNIEXPORT jint JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_AcceptResign
  (JNIEnv * env, jclass class, jint r)
{
  return acceptResign(r);
}


JNIEXPORT jint JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_AcceptDouble
  (JNIEnv *env, jclass class)
{
  return acceptDouble();
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_UpdateMSCubeInfo
  (JNIEnv *env, jclass class, jint nCube, jint fCubeOwner)
{
  updateMSCubeInfo(nCube, fCubeOwner);
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_EvaluateBestMove
  (JNIEnv * env, jclass class, jintArray dices, jintArray move)
{
  jint *ds = (*env)->GetIntArrayElements(env, dices, 0);
  jint *mv = (*env)->GetIntArrayElements(env, move, 0);
  evaluateBestMove(ds, mv);
  (*env)->ReleaseIntArrayElements(env, move, mv, 0);
  (*env)->ReleaseIntArrayElements(env, dices, ds, 0);
}


JNIEXPORT jint JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_AskForResignation
  (JNIEnv *env, jclass class)
{
  return askForResignation();
}


JNIEXPORT jint JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_AskForDoubling
  (JNIEnv *env, jclass class)
{
  return askForDoubling();
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_SetMatchScore
  (JNIEnv *env, jclass class, jint AIScore, jint HumanScore) 
{
  setMatchScore(AIScore, HumanScore);
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_SetGameTurn
  (JNIEnv *env, jclass class, jint fTurn, jint fMove)
{
  setGameTurn(fTurn, fMove);
}


JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_SetMatchTo
  (JNIEnv *env, jclass class, jint nMatchTo)
{
  setMatchTo(nMatchTo); 
}


JNIEXPORT jobjectArray JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_GenerateMoves
  (JNIEnv *env, jclass class, jintArray _b1, jintArray _b2, jint d1, jint d2) 
{
  jint *b1 = (*env)->GetIntArrayElements(env, _b1, 0);
  jint *b2 = (*env)->GetIntArrayElements(env, _b2, 0);

  //jint *mv = (*env)->GetIntArrayElements(env, _mv, 0);

  int b[2][25];
  memcpy(b[0], b1, sizeof(int)*25);
  memcpy(b[1], b2, sizeof(int)*25);

  int nMoves;
  int** moves = generateMoves((ConstTanBoard)b, d1, d2, &nMoves);

  int i = 0;
  jclass c = (*env)->FindClass(env, "[I");
  jobjectArray rows = (*env)->NewObjectArray(env, nMoves, c , NULL);


  for (i=0; i<nMoves; i++) {
    jintArray row = (*env)->NewIntArray(env, 8);
    (*env)->SetIntArrayRegion(env, row, 0, 8, (jint *)moves[i]);
    (*env)->SetObjectArrayElement(env, rows, i, row);
  }

  (*env)->ReleaseIntArrayElements(env, _b1, b1, 0);
  (*env)->ReleaseIntArrayElements(env, _b2, b2, 0);

  for (i=0;i<nMoves;i++) {
    free(moves[i]);
  }
  free(moves);
  
  return rows;
}




JNIEXPORT void JNICALL Java_it_alcacoop_gnubackgammon_logic_GnubgAPI_TestAll
  (JNIEnv *env, jclass class)
{
  setAILevel(SUPREMO);
  testResignation();
  testDoubling();
  testPlayTurn();
}
