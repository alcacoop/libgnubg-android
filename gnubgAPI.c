#include "gnubgAPI.h"
#include "dice.h"

#include <android/log.h>
#define MYLOG(msg) __android_log_write(ANDROID_LOG_ERROR,"MYLOG()",msg);

JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_RollDice
  (JNIEnv *env, jclass class)
{
  char buf[128];
  init_rng();

  //EvalInitialise("gnubg.weights", "gnubg.wd", 0, NULL);
  //InitMatchEquity("zadeh.xml");
  
  MYLOG("\n\n");

  testResignation();
  testDoubling();
  testPlayTurn();

  sprintf(buf, "TEST TERMINATI!");
  return (*env)->NewStringUTF(env, buf);
}
