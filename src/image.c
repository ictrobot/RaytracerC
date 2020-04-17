#include "image.h"
#include <malloc.h>
#include "../lib/lodepng/lodepng.h"
#include <stdint.h>
#include <stdio.h>

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
  uint8_t *data = malloc(image.length * 3 * sizeof(uint8_t));
  for (int y = 0, i = 0, r = 0; y < image.height; y++) {
    for (int x = 0; x < image.width; x++, r++) {
      RGB rgb = image.data[r];
      data[i++] = component(rgb.r);
      data[i++] = component(rgb.g);
      data[i++] = component(rgb.b);
    }
  }
  unsigned int error = lodepng_encode24_file(filename, data, image.width, image.height);
  if (error) {
    printf("Error writing PNG: %u - %s\n", error, lodepng_error_text(error));
    return (int) error;
  }
  return 0;
}
