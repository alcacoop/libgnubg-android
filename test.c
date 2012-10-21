#include "dice.h"
#define TRUE 1

int main (int argc, char** argv) {
  int d[2];
  unsigned long a;
  int b;
  rng _rng = RNG_MERSENNE;
  //rng _rng = RNG_ISAAC;
  rngcontext *_rngctx;
  int i;

  _rngctx = InitRNG(&a, &b, TRUE, _rng);
  printf("%d %d\n", a,b);
	//rcRollout.nSeed ^= 0x792A584B;
  for (i=0; i<10; i++) {
    RollDice(d, &_rng, _rngctx);
    printf("LANCIO: %d %d\n", d[0], d[1]);
  }
}
