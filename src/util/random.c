/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

/* This is xoshiro256+ 1.0, our best and fastest generator for floating-point
   numbers. We suggest to use its upper bits for floating-point
   generation, as it is slightly faster than xoshiro256++/xoshiro256**. It
   passes all tests we are aware of except for the lowest three bits,
   which might fail linearity tests (and just those), so if low linear
   complexity is not considered an issue (as it is usually the case) it
   can be used to generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */


static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

static uint64_t next(uint64_t *s) {
  const uint64_t result = s[0] + s[3];

  const uint64_t t = s[1] << 17;

  s[2] ^= s[0];
  s[3] ^= s[1];
  s[1] ^= s[2];
  s[0] ^= s[3];

  s[2] ^= t;

  s[3] = rotl(s[3], 45);

  return result;
}

///////////////////////////////////////////////////////////////////////////////

#include "random.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

static uint64_t splitmix64(uint64_t *x) {
  uint64_t z = (*x += 0x9e3779b97f4a7c15);
  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
  z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
  return z ^ (z >> 31);
}

RAND rand_init(void) {
  uint64_t x = time(NULL) + rand();
  RAND r;
  for (int i = 0; i < 4; i++) {
    r.s[i] = splitmix64(&x);
  }
  return r;
}

double random(RAND *rand) {
  uint64_t r = next(rand->s);
  return ((double) (r >> 12)) / 0x10000000000000;
}

double random_pm(RAND *rand) {
  double d = random(rand);
  return (d * 2) - 1;
}

Vec3 random_sphere(RAND *rand) {
  double r = random(rand);
  double theta = random(rand) * PI;
  double phi = random(rand) * PI * 2;
  return vec3_coords(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta));
}