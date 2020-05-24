#ifdef _RAYTRACERC_PTHREAD

#include "thread.h"
#include <pthread.h>
#include <sys/sysinfo.h>
#include <malloc.h>
#include <string.h>

void threaded_run(void (*fn)(void *), ThreadArgs *args, int argSize, int maxThreads) {
  int numThreads = get_nprocs();
  if (maxThreads > 0 && numThreads > maxThreads) numThreads = maxThreads;
  if (maxThreads == 1) {
    threaded_single(fn, args);
    return;
  }

  pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
  ThreadArgs **allArgs = malloc(numThreads * sizeof(ThreadArgs *));
  for (int i = 0; i < numThreads; i++) {
    allArgs[i] = malloc(argSize);
    memcpy((void *) allArgs[i], (void *) args, argSize);
    allArgs[i]->threadNum = i;
    allArgs[i]->threadCount = numThreads;
    threaded_init_args(allArgs[i]);

    pthread_create(&threads[i], NULL, (void *) fn, (void *) allArgs[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
    free(allArgs[i]);
  }

  free(threads);
}

#endif
