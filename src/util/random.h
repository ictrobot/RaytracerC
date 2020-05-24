#ifndef RAYTRACERC_RANDOM_H
#define RAYTRACERC_RANDOM_H

#include <stdint.h>
#include "vector3.h"

typedef struct {
  uint64_t s[4];
} RAND;

RAND rand_init(void);

double random(RAND *rand);

double random_pm(RAND *rand);

Vec3 random_sphere(RAND *rand);

#endif //RAYTRACERC_RANDOM_H
