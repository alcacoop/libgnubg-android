#include "gnubgAPI.h"
#include "dice.h"
#include "globals.h"


JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_RollDice
  (JNIEnv *env, jclass class)
{
  char buf[255];
  testAll();

  sprintf(buf, "TEST TERMINATI!");
  return (*env)->NewStringUTF(env, buf);
}
