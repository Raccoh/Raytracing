#ifndef TYPES_H_
#define TYPES_H_

#include <cmath>
#include <algorithm>

#define FEps 1e-6
#define Infty 1e20

// 3-dimensional vector with double-precision
struct vec3d {
  double x = 0, y = 0, z = 0;
  vec3d() {}
  vec3d(double x_, double y_, double z_) { x = x_; y = y_; z = z_; }
  vec3d operator+(const vec3d& b) const { return vec3d(x+b.x,y+b.y,z+b.z); }
  vec3d operator-(const vec3d& b) const { return vec3d(x-b.x,y-b.y,z-b.z); }
  vec3d operator*(double b) const { return vec3d(x*b,y*b,z*b); }
  vec3d operator*(const vec3d& b) const { return vec3d(x*b.x,y*b.y,z*b.z); }
  double length() { return sqrt(x*x + y*y + z*z); }
  double lengthSquared() { double l = length(); return l * l; }
  vec3d& norm() { return *this = *this * (1 / length()); }
  double dot(const vec3d& b) const { return x*b.x+y*b.y+z*b.z; }
  vec3d cross(const vec3d& b) const {
    return vec3d(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
  }
};

// Ray with (o)rigin and (d)irection
struct Ray { vec3d o, d; Ray(vec3d o_, vec3d d_) : o(o_), d(d_) {} };

// Material types
enum Refl_t { DIFF, SPEC };

#endif
