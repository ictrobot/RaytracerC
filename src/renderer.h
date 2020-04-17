#include "image.h"
#include "objects/scene.h"

#ifndef RAYTRACERC_RENDERER_H
#define RAYTRACERC_RENDERER_H

Image render(int width, int height, double fov, Scene *scene);

#define RAYTRACERC_MULTI
#ifdef RAYTRACERC_MULTI

Image renderMulti(int width, int height, double fov, Scene *scene);

#endif

#endif //RAYTRACERC_RENDERER_H
