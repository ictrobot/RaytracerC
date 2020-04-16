#include "image.h"
#include <stddef.h>
#include <malloc.h>
#include "../lib/libattopng/libattopng.h"

Image img_new(int width, int height) {
  size_t sizeImageData = sizeof(RGB) * width * height;
  RGB *data = malloc(sizeImageData);
  return (Image) {width, height, width * height, data};
}

void img_setPx(Image image, int x, int y, RGB colour) {
  image.data[x + (y * image.width)] = colour;
}

static uint8_t component(double d) {
  if (d <= 0)
    return 0;
  if (d >= 1)
    return 255;
  return (uint8_t) (d * 255.0);
}

int img_save(Image image, char *filename) {
  libattopng_t *png = libattopng_new(image.width, image.height, PNG_RGB);
  for (int y = 0, i = 0; y < image.height; y++) {
    for (int x = 0; x < image.width; x++, i++) {
      RGB rgb = image.data[i];
      uint8_t r = component(rgb.r);
      uint8_t g = component(rgb.g);
      uint8_t b = component(rgb.b);
      uint32_t packed = r | (g << 8u) | (b << 16u) | (0xFFu < 24u);
      libattopng_set_pixel(png, x, y, packed);
    }
  }
  int result = libattopng_save(png, filename);
  libattopng_destroy(png);
  return result;
}
