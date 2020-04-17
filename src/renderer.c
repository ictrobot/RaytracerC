#include "renderer.h"
#include "camera.h"

static const double EPSILON = 0.001;

static RaycastHit findClosestHit(Scene *scene, Ray ray) {
  RaycastHit closest = raycast_miss();
  for (int i = 0; i < scene->lenObj; i++) {
    RaycastHit trial = obj_intersectWith(scene->objects[i], ray);
    if (trial.dist < closest.dist) closest = trial;
  }
  return closest;
}

static RGB illuminate(Scene *scene, Ray ray, RaycastHit hit) {
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

    Ray shadowRay = (Ray) {vec3_add(hit.location, vec3_scaleConst(L, EPSILON)), L};
    RaycastHit shadowHit = findClosestHit(scene, shadowRay);

    if (shadowHit.dist > distanceToLight) {
      if (NdotL > 0) {
        RGB diffuse = rgb_scaleConst(rgb_scale(rgb_scaleConst(hit.object->colour, hit.object->phong_kD), I), NdotL);
        result = rgb_add(result, diffuse);
      }

      if (RdotV > 0 && hit.object->phong_kS > 0) {
        RGB specular = rgb_scaleConst(rgb_scale(rgb_scaleConst(light->colour, hit.object->phong_kS), I),
                                      pow(RdotV, hit.object->phong_alpha));
        result = rgb_add(result, specular);
      }
    }
  }
  return result;
}

static RGB trace(Scene *scene, Ray ray, int bouncesLeft) {
  RaycastHit hit = findClosestHit(scene, ray);
  if (hit.object == NULL) return scene->backgroundColor;

  RGB directIllumination = illuminate(scene, ray, hit);
  if (bouncesLeft <= 0 || hit.object->reflectivity <= 0) {
    return directIllumination;
  } else {
    directIllumination = rgb_scaleConst(directIllumination, 1 - hit.object->reflectivity);

    Vec3 R = vec3_norm(vec3_reflect(vec3_scaleConst(ray.direction, -1), hit.normal));
    Ray reflectedRay = (Ray) {vec3_add(hit.location, vec3_scaleConst(R, EPSILON)), R};
    RGB reflectedIllumination = trace(scene, reflectedRay, bouncesLeft - 1);
    reflectedIllumination = rgb_scaleConst(reflectedIllumination, hit.object->reflectivity);

    return rgb_add(directIllumination, reflectedIllumination);
  }
}

Image render(int width, int height, double fov, Scene *scene) {
  Image result = img_new(width, height);
  Camera *camera = camera_new(width, height, fov);
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      Ray ray = camera_cast(camera, x, y);
      RGB rgb = trace(scene, ray, scene->bounces);
      img_setPx(result, x, y, rgb);
    }
  }
  return result;
}