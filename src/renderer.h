#include "image.h"
#include "objects/scene.h"

#ifndef RAYTRACERC_RENDERER_H
#define RAYTRACERC_RENDERER_H

Image render(int width, int height, double fov, Scene *scene, int maxThreads);

#endif //RAYTRACERC_RENDERER_H
