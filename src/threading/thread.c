#include "thread.h"

void threaded_single(void (*fn)(void *), ThreadArgs *args) {
  args->threadNum = 1;
  args->threadCount = 1;
  fn((void *) args);
}

#ifndef _RAYTRACERC_PTHREAD
#ifndef _RAYTRACERC_WIN32
void threaded_run(void (*fn) (void *), ThreadArgs *args, int argSize, int maxThreads) {
  threaded_single(fn, args);
}
#endif
#endif