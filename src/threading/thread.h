#ifndef RAYTRACERC_THREAD_H
#define RAYTRACERC_THREAD_H

#include "../util/random.h"

typedef struct {
  int threadNum;
  int threadCount;
  RAND threadRand;
} ThreadArgs;

void threaded_run(void (*fn)(void *), ThreadArgs *args, int argSize, int maxThreads);

void threaded_single(void (*fn)(void *), ThreadArgs *args);

void threaded_init_args(ThreadArgs *args);

#endif //RAYTRACERC_THREAD_H
