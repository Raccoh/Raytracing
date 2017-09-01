#include "Types.h"

struct Sphere {
  double rad; vec3d pos; vec3d emission, color; Refl_t refl_t;

  Sphere(double rad_, vec3d pos_, vec3d emission_, vec3d color_,
         Refl_t refl_t_)
      : rad(rad_), pos(pos_), emission(emission_), color(color_),
        refl_t(refl_t_) {}

  double intersect(const Ray& r) const {
    double b = r.d.dot(r.o - pos) * 2;
    double c = (r.o - pos).lengthSquared() - (rad * rad);
    double discriminant = b * b - (4 * c);
    if (discriminant < 0) return 0;
    if (discriminant < FEps) return -b / 2;
    double t_0 = (-b + sqrt(discriminant)) / 2;
    double t_1 = (-b - sqrt(discriminant)) / 2;
    if (t_0 < t_1) {
      if (t_0 > 0) return t_0;
      if (t_1 > 0) return t_1;
    } else {
      if (t_1 > 0) return t_1;
      if (t_0 > 0) return t_0;
    }
    return 0;
  }
};
