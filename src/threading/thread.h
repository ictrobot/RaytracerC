#ifndef RAYTRACERC_THREAD_H
#define RAYTRACERC_THREAD_H

typedef struct {
  int threadNum;
  int threadCount;
} ThreadArgs;

void threaded_run(void (*fn)(void *), ThreadArgs *args, int argSize, int maxThreads);

void threaded_single(void (*fn)(void *), ThreadArgs *args);

#endif //RAYTRACERC_THREAD_H
