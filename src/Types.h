#ifndef TYPES_H_
#define TYPES_H_

#include <cmath>
#include <algorithm>

#define FEps 1e-6
#define Infty 1e20

struct vec3d {
  double x = 0, y = 0, z = 0;
  vec3d() {}
  vec3d(double x_, double y_, double z_) {
    x = x_; y = y_; z = z_;
  }
  vec3d operator+(const vec3d& b) const { return vec3d(x+b.x,y+b.y,z+b.z); }
  vec3d operator-(const vec3d& b) const { return vec3d(x-b.x,y-b.y,z-b.z); }
  vec3d operator*(double b) const { return vec3d(x*b,y*b,z*b); }
  vec3d mult(const vec3d& b) const { return vec3d(x*b.x,y*b.y,z*b.z); }
  double length() { return sqrt(x*x + y*y + z*z); }
  double lengthSquared() { double l = length(); return l * l; }
  vec3d& norm() { return *this = *this * (1 / length()); }
  double dot(const vec3d& b) const { return x*b.x+y*b.y+z*b.z; }
  vec3d cross(const vec3d& b) const {
    return vec3d(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);
  }
};

struct color4i {
  int r = 0, g = 0, b = 0, a = 255;
  color4i() {}
  color4i(int r_, int g_, int b_, int a_ = 255) {
    r = std::max(0, std::min(255, r_));
    g = std::max(0, std::min(255, g_));
    b = std::max(0, std::min(255, b_));
    a = std::max(0, std::min(255, a_));
  }
  color4i(double r_, double g_, double b_, double a_ = 255) {
    *this = color4i((int)(r_ * 255 + 0.5), (int)(g_ * 255 + 0.5),
                    (int)(b_ * 255 + 0.5), (int)(a_ * 255 + 0.5));
  }
  color4i operator+(const color4i& c) const {
    return color4i(r+c.r, g+c.g, b+c.b, a+c.a);
  }
  color4i operator-(const color4i& c) const {
    return color4i(r-c.r, g-c.g, b-c.b, a-c.a);
  }
  color4i operator*(double s) const {
    return color4i((int)(r*s + 0.5), (int)(g*s + 0.5), (int)(b*s + 0.5));
  }
  color4i operator*(const color4i& c) const {
    return color4i((r/255.0) * (c.r/255.0),
                   (g/255.0) * (c.g/255.0),
                   (b/255.0) * (c.b/255.0),
                   (a/255.0) * (c.a/255.0));
  }
};

struct Ray { vec3d o, d; Ray(vec3d o_, vec3d d_) : o(o_), d(d_) {} };
enum Refl_t { DIFF, SPEC, REFR };  // material types

#endif
