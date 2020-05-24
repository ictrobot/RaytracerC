#include "objects/scene.h"
#include "renderer.h"
#include "stdio.h"
#include <time.h>

const static int width = 1920;
const static int height = 1080;

int main() {
  Scene *scene = scene_make();
  printf("Initialized scene with %i objects and %i lights\n", scene->lenObj, scene->lenLight);

  printf("Starting %ix%i render\n", width, height);
  clock_t t = clock();
  Image image = render(width, height, 60, scene, 0);
  t = clock() - t;
  double time_taken = ((double) t) / CLOCKS_PER_SEC;
  printf("Rendered scene in %.3f seconds\n", time_taken);

  t = clock();
  int returnCode = img_save(image, "render.png");

  if (returnCode == 0) {
    t = clock() - t;
    time_taken = ((double) t) / CLOCKS_PER_SEC;
    printf("Saved PNG in %.3f seconds\n", time_taken);
  } else {
    fprintf(stderr, "Failed to save PNG");
  }

  scene_free(scene);
  return returnCode;
}
