#include "vector3.h"
#include "math.h"

Vec3 vec3_val(double d) {
  return (Vec3) {d, d, d};
}

Vec3 vec3_coords(double x, double y, double z) {
  return (Vec3) {x, y, z};
}

Vec3 vec3_add(Vec3 v, Vec3 other) {
  return (Vec3) {v.x + other.x, v.y + other.y, v.z + other.z};
}

Vec3 vec3_addConst(Vec3 v, double d) {
  return (Vec3) {v.x + d, v.y + d, v.z + d};
}

Vec3 vec3_sub(Vec3 v, Vec3 other) {
  return (Vec3) {v.x - other.x, v.y - other.y, v.z - other.z};
}

Vec3 vec3_scaleConst(Vec3 v, double d) {
  return (Vec3) {v.x * d, v.y * d, v.z * d};
}

Vec3 vec3_scale(Vec3 v, Vec3 other) {
  return (Vec3) {v.x * other.x, v.y * other.y, v.z * other.z};
}

double vec3_dot(Vec3 v, Vec3 other) {
  return (v.x * other.x) + (v.y * other.y) + (v.z * other.z);
}

Vec3 vec3_cross(Vec3 v, Vec3 other) {
  return (Vec3) {v.y * other.z - v.z * other.y, v.z * other.x - v.x * other.z, v.x * other.y - v.y * other.x};
}

Vec3 vec3_power(Vec3 v, double d) {
  return (Vec3) {pow(v.x, d), pow(v.y, d), pow(v.z, d)};
}

Vec3 vec3_inv(Vec3 v) {
  return (Vec3) {1 / v.x, 1 / v.y, 1 / v.z};
}

double vec3_magnitude2(Vec3 v) {
  return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

double vec3_magnitude(Vec3 v) {
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

Vec3 vec3_norm(Vec3 v) {
  return vec3_scaleConst(v, 1 / vec3_magnitude(v));
}

Vec3 vec3_reflect(Vec3 v, Vec3 normal) {
  return vec3_sub(vec3_scaleConst(normal, 2 * vec3_dot(v, normal)), v);
}

// Vec3 vec3_randomDir();