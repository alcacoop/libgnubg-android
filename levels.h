#include "eval.h"

#ifndef LEVELS_H
#define LEVELS_H
typedef enum _available_levels {
  BEGINNER,
  CASUAL,
  INTERMEDIATE,
  ADVANCED,
  EXPERT,
  WORLDCLASS,
  SUPREMO,
  GRANDMASTER
} available_levels;


typedef struct _level {
  evalcontext ec;
  movefilter mf[4][4];
} level;

extern level levels[];
#endif
