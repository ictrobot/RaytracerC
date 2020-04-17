#ifdef _RAYTRACERC_WIN32
#include "thread.h"
#include <windows.h>
#include <process.h>
#include <malloc.h>
#include <string.h>


void threaded_run(void (*fn) (void *), ThreadArgs *args, int argSize, int maxThreads) {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  int numThreads = sysinfo.dwNumberOfProcessors;
  if (maxThreads > 0 && numThreads > maxThreads) numThreads = maxThreads;
  if (maxThreads == 1) {
    threaded_single(fn, args);
    return;
  }

  HANDLE *threads = malloc(numThreads * sizeof(HANDLE));
  ThreadArgs **allArgs = malloc(numThreads * sizeof(ThreadArgs *));
  for (int i = 0; i < numThreads; i++) {
    allArgs[i] = malloc(argSize);
    memcpy((void *) allArgs[i], (void *) args, argSize);
    allArgs[i]->threadNum = i;
    allArgs[i]->threadCount = numThreads;

    threads[i] = (HANDLE) _beginthread(fn, 0, (void *) allArgs[i]);
  }

  for (int i = 0; i < numThreads; i++) {
    WaitForSingleObject(threads[i], INFINITE);
    free(allArgs[i]);
  }

  free(threads);
}


#endif