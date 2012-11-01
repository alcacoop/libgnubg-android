#include "jniAPI.h"
#include "globals.h"
#include "nativeAPI.h"



JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_RollDice
  (JNIEnv * env, jclass class, jintArray dices) 
{
  jint *ds = (*env)->GetIntArrayElements(env, dices, 0);
  rollDice(ds);
  (*env)->ReleaseIntArrayElements(env, dices, ds, 0);
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_InitializeEnvironment
  (JNIEnv *env, jclass class)
{
  initEnvironment();
  MYLOG("EVALUATOR INITIALIZED");
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetAILevel
  (JNIEnv * env, jclass class, jint level)
{
  setAILevel(level);
  MYLOG("AI LEVEL SETTED");
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetBoard
  (JNIEnv * env, jclass class, jintArray _b1, jintArray _b2) 
{
  jint *b1 = (*env)->GetIntArrayElements(env, _b1, 0);
  jint *b2 = (*env)->GetIntArrayElements(env, _b2, 0);

  int b[2][25];
  memcpy(b[0], b1, sizeof(int)*25);
  memcpy(b[1], b2, sizeof(int)*25);
  setBoard((ConstTanBoard)b);
  printBoard(msBoard());

  (*env)->ReleaseIntArrayElements(env, _b1, b1, 0);
  (*env)->ReleaseIntArrayElements(env, _b2, b2, 0);
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AcceptResign
  (JNIEnv * env, jclass class, jint r)
{
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;
  
  return acceptResign(r);
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AcceptDouble
  (JNIEnv *env, jclass class)
{
  ms.fMove = 1;
  ms.fTurn = 0;
  ms.anScore[0] = 0;
  ms.anScore[1] = 0;
  ms.nMatchTo = 7;

  return acceptDouble();
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_UpdateMSCubeInfo
  (JNIEnv *env, jclass class, jint nCube, jint fCubeOwner)
{
  updateMSCubeInfo(nCube, fCubeOwner);
  char buf[20];
  sprintf(buf, "N: %d - O: %d", ms.nCube, ms.fCubeOwner);
  MYLOG(buf);
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_EvaluateBestMove
  (JNIEnv * env, jclass class, jintArray dices, jintArray move)
{
  jint *ds = (*env)->GetIntArrayElements(env, dices, 0);
  jint *mv = (*env)->GetIntArrayElements(env, move, 0);
  evaluateBestMove(ds, mv);
  (*env)->ReleaseIntArrayElements(env, move, mv, 0);
  (*env)->ReleaseIntArrayElements(env, dices, ds, 0);
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AskForResignation
  (JNIEnv *env, jclass class)
{
  ms.fMove = 0;
  ms.fTurn = 0;
  return askForResignation();
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AskForDoubling
  (JNIEnv *env, jclass class)
{
  ms.fMove = 0;
  ms.fTurn = 0;
  return askForDoubling();
}
