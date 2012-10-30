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


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_SetIALevel
  (JNIEnv * env, jclass class, jint level)
{
  setAILevel(level);
  MYLOG("AI LEVEL SETTED");
}
