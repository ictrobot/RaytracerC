#ifndef RAYTRACERC_RAYCAST_H
#define RAYTRACERC_RAYCAST_H

#include "util/vector3.h"
#include <stddef.h>

typedef struct {
  Vec3 origin;
  Vec3 direction;
} Ray;

Vec3 ray_eval(Ray ray, double distance);

typedef struct object Object;
typedef struct {
  double dist;
  Object *object;
  Vec3 location;
  Vec3 normal;
} RaycastHit;

RaycastHit raycast_miss();

#endif //RAYTRACERC_RAYCAST_H
