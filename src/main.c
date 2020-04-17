#include "objects/scene.h"
#include "renderer.h"

int main() {
  Scene *scene = scene_make();
  Image image = render(1920, 1080, 60, scene, -1);
  return img_save(image, "test.png");
}
