#include "light.h"
#include <math.h>
#include <malloc.h>

Light *light_new(Vec3 pos, RGB colour, double intensity) {
  Light *light = malloc(sizeof(Light));
  light->pos = pos;
  light->colour = colour;
  light->intensity = intensity;
  return light;
}

RGB light_getIlluminationAt(Light *light, double distance) {
  double scale = light->intensity / (M_PI * 4 * distance * distance);
  return rgb_scaleConst(light->colour, scale);
}
