#ifndef RAYTRACERC_VECTOR3_H
#define RAYTRACERC_VECTOR3_H

#define PI 3.14159265358979323846

typedef struct {
  double x;
  double y;
  double z;
} Vec3;

Vec3 vec3_val(double d);

Vec3 vec3_coords(double x, double y, double z);

Vec3 vec3_add(Vec3 v, Vec3 other);

Vec3 vec3_addConst(Vec3 v, double d);

Vec3 vec3_sub(Vec3 v, Vec3 other);

Vec3 vec3_scaleConst(Vec3 v, double d);

Vec3 vec3_scale(Vec3 v, Vec3 other);

double vec3_dot(Vec3 v, Vec3 other);

Vec3 vec3_cross(Vec3 v, Vec3 other);

Vec3 vec3_power(Vec3 v, double d);

Vec3 vec3_inv(Vec3 v);

double vec3_magnitude2(Vec3 v);

double vec3_magnitude(Vec3 v);

Vec3 vec3_norm(Vec3 v);

Vec3 vec3_reflect(Vec3 v, Vec3 normal);

// Vec3 vec3_randomDir();

#endif //RAYTRACERC_VECTOR3_H
