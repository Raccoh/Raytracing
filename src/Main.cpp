#include <cstdio>

#include "Types.h"
#include "Shapes.h"

Sphere spheres[] = {
  Sphere(1e5, vec3d( 1e5+1,40.8,81.6), color4i(.75,.25,.25)),  // Left
  Sphere(1e5, vec3d(-1e5+99,40.8,81.6),color4i(.25,.25,.75)),  // Right
  Sphere(1e5, vec3d(50,40.8, 1e5),     color4i(.75,.75,.75)),  // Back
  Sphere(1e5, vec3d(50,40.8,-1e5+170), color4i()),             // Front
  Sphere(1e5, vec3d(50, 1e5, 81.6),    color4i(.75,.75,.75)),  // Bottom
  Sphere(1e5, vec3d(50,-1e5+81.6,81.6),color4i(.75,.75,.75)),  // Top
  Sphere(16.5,vec3d(27, 16.5, 47),     color4i(0.5,0.5,0.5)),  // Mirror
  Sphere(16.5,vec3d(73,16.5,78),       color4i(1.0,1.0,1.0))   // Glas
};
int numSpheres = sizeof(spheres) / sizeof(Sphere);

vec3d plp(50, 80, 81.6);     // point light position
color4i plc(1.0, 1.0, 1.0);  // point light color

color4i radiance(const Sphere& s, const Ray& r) {
  double toLightDist = (plp - r.o).length();
  bool inShadow = false;
  for (int i = 0; i < numSpheres; ++i) {
    double t = spheres[i].intersect(r);
    if (t > 0 && t < toLightDist) {
      inShadow = true;
      break;
    }
  }
  color4i result = plc * s.color * 0.33;  // Ambient lighting
  if (inShadow)
    return result;
  // Add diffuse lighting
  vec3d normal = (r.o - s.pos).norm();
  // Careful: abs is effectively two-sided lighting, normally it's max(0, dot)
  result = result + (plc * s.color * std::abs(r.d.dot(normal)) * 0.67);
  return result;
}

void writeImage(color4i* pixels, int width, int height) {
  FILE* f = fopen("image.ppm", "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      int i = row * width + col;
      fprintf(f, "%d %d %d ", pixels[i].r, pixels[i].g, pixels[i].b);
    }
  }
  fclose(f);
}

int main()
{
  // Render the smallpt scene with a point light source.
  // Coordinates taken from smallpt.
  int w = 1024, h = 768;
  Ray cam(vec3d(50, 52, 295.6), vec3d(0, -0.042612, -1).norm());
  vec3d cx = vec3d(w*.5135/h, 0, 0), cy = (cx.cross(cam.d)).norm()*.5135;
  color4i* pixels = new color4i[w * h];
  for (int row = 0; row < h; ++row) {
    for (int col = 0; col < w; ++col) {
      vec3d d = cx * ((double)col/w - 0.5) + cy * ((double)row/h - 0.5) + cam.d;
      Ray r(cam.o + (d * 130), d.norm());
      double closestT = Infty;
      int closestId = -1;
      for (int i = 0; i < numSpheres; ++i) {
        double t = spheres[i].intersect(r);
        if (t > 0 && t < closestT) {
          closestT = t;
          closestId = i;
        }
      }
      int i = (h - row - 1) * w + col;
      if (closestT < Infty) {
        vec3d newO = r.o + (r.d * closestT);
        vec3d toLight = (plp - newO).norm();
        pixels[i] = radiance(spheres[closestId], Ray(newO + toLight, toLight));
      }                    // avoids false self-shadowing ^^^^^^^^^
    }
  }
  writeImage(pixels, w, h);
}
