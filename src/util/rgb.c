#include "rgb.h"
#include "math.h"

RGB rgb_val(double d) {
  return (RGB) {d, d, d};
}

RGB rgb_ints(int r, int g, int b) {
  return (RGB) {(double) r / 255.0, (double) g / 255.0, (double) b / 255.0};
}

RGB rgb_vals(double r, double g, double b) {
  return (RGB) {r, g, b};
}

RGB rgb_add(RGB x, RGB y) {
  return (RGB) {x.r + y.r, x.g + y.g, x.b + y.b};;
}

RGB rgb_addConst(RGB x, double d) {
  return (RGB) {x.r + d, x.g + d, x.b + d};
}

RGB rgb_scaleConst(RGB x, double s) {
  return (RGB) {x.r * s, x.g * s, x.b * s};
}

RGB rgb_scale(RGB x, RGB other) {
  return (RGB) {x.r * other.r, x.g * other.g, x.b * other.b};
}