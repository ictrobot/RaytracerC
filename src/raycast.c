#define _USE_MATH_DEFINES

#include <math.h>
#include "raycast.h"

Vec3 ray_eval(Ray ray, double distance) {
  return vec3_add(ray.origin, vec3_scaleConst(ray.direction, distance));
}

RaycastHit raycast_miss() {
  return (RaycastHit) {INFINITY, NULL, vec3_val(0), vec3_val(0)};
}
