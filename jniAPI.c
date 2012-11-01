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
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetAILevel
  (JNIEnv * env, jclass class, jint level)
{
  setAILevel(level);
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

  (*env)->ReleaseIntArrayElements(env, _b1, b1, 0);
  (*env)->ReleaseIntArrayElements(env, _b2, b2, 0);
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AcceptResign
  (JNIEnv * env, jclass class, jint r)
{
  return acceptResign(r);
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AcceptDouble
  (JNIEnv *env, jclass class)
{
  return acceptDouble();
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_UpdateMSCubeInfo
  (JNIEnv *env, jclass class, jint nCube, jint fCubeOwner)
{
  updateMSCubeInfo(nCube, fCubeOwner);
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
  return askForResignation();
}


JNIEXPORT jint JNICALL Java_com_example_testndk_gnubgAPI_AskForDoubling
  (JNIEnv *env, jclass class)
{
  return askForDoubling();
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetMatchScore
  (JNIEnv *env, jclass class, jint AIScore, jint HumanScore) 
{
  setMatchScore(AIScore, HumanScore);
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetGameTurn
  (JNIEnv *env, jclass class, jint fTurn, jint fMove)
{
  setGameTurn(fTurn, fMove);
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetMatchTo
  (JNIEnv *env, jclass class, jint nMatchTo)
{
  setMatchTo(nMatchTo); 
}


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_TestAll
  (JNIEnv *env, jclass class)
{
  testAll();
  testAll();
}
