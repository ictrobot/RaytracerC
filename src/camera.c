#define _USE_MATH_DEFINES

#include <math.h>
#include "camera.h"
#include <malloc.h>

static inline double rad(double degrees) {
  return degrees / 180.0 * M_PI;
}

Camera *camera_new(int width, int height, double fov) {
  Camera *camera = malloc(sizeof(Camera));

  camera->width_px = width;
  camera->height_px = height;

  camera->aspectRatio = ((double) width) / ((double) height);

  camera->width_m = 2 * tan(rad(fov) / 2);
  camera->height_m = camera->width_m / camera->aspectRatio;

  camera->x_step_m = camera->width_m / width;
  camera->y_step_m = camera->height_m / height;

  return camera;
}

Ray camera_cast(Camera *camera, double x, double y) {
  double x_pos = (camera->x_step_m - camera->width_m) / 2 + x * camera->x_step_m;
  double y_pos = (camera->y_step_m + camera->height_m) / 2 - y * camera->y_step_m;
  return (Ray) {vec3_val(0), vec3_norm(vec3_coords(x_pos, y_pos, 1))};
}