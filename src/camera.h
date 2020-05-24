#include "raycast.h"

#ifndef RAYTRACERC_CAMERA_H
#define RAYTRACERC_CAMERA_H

typedef struct {
  int width_px, height_px;
  double width_m, height_m;
  double fov; // 45 for 4:3, 60 for 16:9
  double aspectRatio;
  double x_step_m, y_step_m;
} Camera;

Camera *camera_new(int width, int height, double fov);

Ray camera_cast(Camera *camera, double x, double y);

#endif //RAYTRACERC_CAMERA_H
