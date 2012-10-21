#include "gnubgAPI.h"
#include "dice.h"

JNIEXPORT void JNICALL Java_com_example_testndk_gnubgAPI_RollDice
  (JNIEnv *env, jclass class)
{
  char buf[128];
  int dices[2];
  unsigned long *a;
  int *b;
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  rngcontext *_rngctx;


  /*
  const jbyte *str;
  str = (*env)->GetStringUTFChars(env, prompt, NULL);
  if (str == NULL) {
    return NULL; 
  }
  (*env)->ReleaseStringUTFChars(env, prompt, str);
  */


  _rngctx = InitRNG(a, b, 1, _rng);
	//rcRollout.nSeed ^= 0x792A584B;
  RollDice(dices, &_rng, _rngctx);
  sprintf(buf, "DADI: %d  %d", dices[0], dices[1]);
  return (*env)->NewStringUTF(env, buf);
}
