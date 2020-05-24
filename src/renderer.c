#include <math.h>
#include "renderer.h"
#include "camera.h"
#include <malloc.h>
#include "threading/thread.h"

static const double EPSILON = 0.001;

static RaycastHit findClosestHit(Scene *scene, Ray ray) {
  RaycastHit closest = raycast_miss();
  for (int i = 0; i < scene->lenObj; i++) {
    RaycastHit trial = obj_intersectWith(scene->objects[i], ray);
    if (trial.dist < closest.dist) closest = trial;
  }
  return closest;
}

static RGB illuminate(Scene *scene, Ray ray, RaycastHit hit, RAND *rand) {
  RGB result = rgb_scale(hit.object->colour, scene->ambientLight);

  for (int i = 0; i < scene->lenLight; i++) {
    Light *light = scene->lights[i];

    double distanceToLight = vec3_magnitude(vec3_sub(light->pos, hit.location));
    RGB I = light_getIlluminationAt(light, distanceToLight);

    Vec3 V = vec3_norm(vec3_sub(ray.origin, hit.location)); // ray.direction;
    Vec3 L = vec3_norm(vec3_sub(light->pos, hit.location));
    Vec3 R = vec3_norm(vec3_reflect(L, hit.normal));

    double NdotL = vec3_dot(hit.normal, L);
    double RdotV = vec3_dot(R, V);
    if (NdotL <= 0 && RdotV <= 0) continue;

    int notShadowed = 0;
    int shadowRays = scene->lightSize > 0 ? scene->shadowRays : 1;
    for (int c = 0; c < shadowRays; c++) {
      Vec3 random = vec3_scaleConst(random_sphere(rand), scene->lightSize);
      Vec3 dir = vec3_norm(vec3_sub(vec3_add(light->pos, random), hit.location));
      Ray shadowRay = (Ray) {vec3_add(hit.location, vec3_scaleConst(dir, EPSILON)), dir};
      RaycastHit shadowHit = findClosestHit(scene, shadowRay);
      if (shadowHit.dist > distanceToLight) notShadowed++;
    }

    if (notShadowed > 0) {
      double shadowRatio = (double) notShadowed / shadowRays;
      if (NdotL > 0) {
        RGB diffuse = rgb_scaleConst(rgb_scale(rgb_scaleConst(hit.object->colour, hit.object->phong_kD), I),
                                     NdotL * shadowRatio);
        result = rgb_add(result, diffuse);
      }

      if (RdotV > 0 && hit.object->phong_kS > 0) {
        RGB specular = rgb_scaleConst(rgb_scale(rgb_scaleConst(light->colour, hit.object->phong_kS), I),
                                      pow(RdotV, hit.object->phong_alpha) * shadowRatio);
        result = rgb_add(result, specular);
      }
    }
  }
  return result;
}

static RGB trace(Scene *scene, Ray ray, int bouncesLeft, RAND *rand) {
  RaycastHit hit = findClosestHit(scene, ray);
  if (hit.object == NULL) return scene->backgroundColor;

  RGB directIllumination = illuminate(scene, ray, hit, rand);
  if (bouncesLeft <= 0 || hit.object->reflectivity <= 0) {
    return directIllumination;
  } else {
    directIllumination = rgb_scaleConst(directIllumination, 1 - hit.object->reflectivity);

    Vec3 R = vec3_norm(vec3_reflect(vec3_scaleConst(ray.direction, -1), hit.normal));
    Ray reflectedRay = (Ray) {vec3_add(hit.location, vec3_scaleConst(R, EPSILON)), R};
    RGB reflectedIllumination = trace(scene, reflectedRay, bouncesLeft - 1, rand);
    reflectedIllumination = rgb_scaleConst(reflectedIllumination, hit.object->reflectivity);

    return rgb_add(directIllumination, reflectedIllumination);
  }
}

static RGB traceRay(Scene *scene, Ray ray, RAND *rand) {
  if (scene->dofAmount == 0) {
    return trace(scene, ray, scene->bounces, rand);
  } else {
    Vec3 focalPoint = ray_eval(ray, scene->dofDistance);
    RGB value = rgb_val(0);
    for (int c = 0; c < scene->dofRays; c++) {
      Vec3 origin = ray.origin;
      origin.x += random_pm(rand) * scene->dofAmount;
      origin.y += random_pm(rand) * scene->dofAmount;
      Ray dofRay = (Ray) {origin, vec3_norm(vec3_sub(focalPoint, origin))};
      value = rgb_add(value, trace(scene, dofRay, scene->bounces, rand));
    }
    return rgb_scaleConst(value, 1.0 / scene->dofRays);
  }
}

typedef struct {
  ThreadArgs thread;
  Camera *camera;
  Scene *scene;
  Image image;
} RenderThreadArgs;

static void renderThread(void *args) {
  RenderThreadArgs *rtArgs = args;

  for (int x = 0; x < rtArgs->image.width; x++) {
    for (int y = rtArgs->thread.threadNum; y < rtArgs->image.height; y += rtArgs->thread.threadCount) {
      RGB value;

      if (rtArgs->scene->samples == 0) {
        Ray ray = camera_cast(rtArgs->camera, x, y);
        value = traceRay(rtArgs->scene, ray, &rtArgs->thread.threadRand);
      } else {
        value = rgb_val(0);
        RAND *rand = &rtArgs->thread.threadRand;

        int jitteredSize = (int) sqrt(rtArgs->scene->samples);
        for (int iX = 0; iX < jitteredSize; iX++) {
          double offsetX = (iX + random(rand)) / jitteredSize;
          for (int iY = 0; iY < jitteredSize; iY++) {
            double offsetY = (iY + random(rand)) / jitteredSize;
            Ray ray = camera_cast(rtArgs->camera, x - 0.5 + offsetX, y - 0.5 + offsetY);
            value = rgb_add(value, traceRay(rtArgs->scene, ray, &rtArgs->thread.threadRand));
          }
        }

        int remainingSamples = rtArgs->scene->samples - (jitteredSize * jitteredSize);
        for (int i = 0; i < remainingSamples; i++) {
          Ray ray = camera_cast(rtArgs->camera, x - 0.5 + random(rand), y - 0.5 + random(rand));
          value = rgb_add(value, traceRay(rtArgs->scene, ray, &rtArgs->thread.threadRand));
        }

        value = rgb_scaleConst(value, 1.0 / rtArgs->scene->samples);
      }
      img_setPx(rtArgs->image, x, y, value);
    }
  }
}

Image render(int width, int height, double fov, Scene *scene, int maxThreads) {
  RenderThreadArgs *args = malloc(sizeof(RenderThreadArgs));
  args->scene = scene;
  args->camera = camera_new(width, height, fov);
  Image image = args->image = img_new(width, height);

  threaded_run(renderThread, &args->thread, sizeof(*args), maxThreads);

  free(args->camera);
  free(args);

  return image;
}