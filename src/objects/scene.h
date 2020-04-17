#ifndef RAYTRACERC_SCENE_H
#define RAYTRACERC_SCENE_H

#include "objects.h"
#include "light.h"

typedef struct {
  Object **objects;
  int lenObj;
  Light **lights;
  int lenLight;
  RGB ambientLight;
  RGB backgroundColor;
  // quality settings
  int bounces;
} Scene;

Scene *scene_make(void);

void scene_free(Scene *scene);

#endif //RAYTRACERC_SCENE_H
