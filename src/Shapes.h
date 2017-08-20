#include <cstdio>

#include "Types.h"

struct Sphere {
  double rad; vec3d pos; color4i color;
  Sphere(double rad_, vec3d pos_, color4i color_)
      : rad(rad_), pos(pos_), color(color_) {}
  double intersect(const Ray& r) const {
    double b = r.d.dot(r.o - pos) * 2;
    double c = (r.o - pos).lengthSquared() - (rad * rad);
    double discriminant = b * b - (4 * c);
    if (discriminant < 0)
      return 0;
    if (discriminant < FEps)
      return -b / 2;
    double t_0 = (-b + sqrt(discriminant)) / 2;
    double t_1 = (-b - sqrt(discriminant)) / 2;
    // Careful: The following does not account for 'camera is inside the sphere'
    if (t_0 < t_1 && t_0 > 0)
      return t_0;
    if (t_1 < t_0 && t_1 > 0)
      return t_1;
    return 0;
  }
};
