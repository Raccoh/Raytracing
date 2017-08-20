#include <cstdio>

#include "Types.h"
#include "Shapes.h"

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
  // Render the mirror ball from smallpt fully blue on gray background (no light
  // source yet). Coordinates taken from smallpt.
  int w = 1024, h = 768;
  Sphere s(16.5, vec3d(27, 16.5, 47), color4i(0.0, 0.0, 1.0));
  Ray cam(vec3d(50,52,295.6), vec3d(0,-0.042612,-1).norm());
  vec3d cx = vec3d(w*.5135/h, 0, 0), cy = (cx.cross(cam.d)).norm()*.5135;
  color4i* pixels = new color4i[w * h];
  for (int row = 0; row < h; ++row) {
    for (int col = 0; col < w; ++col) {
      vec3d d = cx * ((double)col/w - 0.5) + cy * ((double)row/h - 0.5) + cam.d;
      if (s.intersect(Ray(cam.o+d*140,d.norm())))
        pixels[(h - row - 1) * w + col] = s.color;
      else
        pixels[(h - row - 1) * w + col] = color4i(0.5, 0.5, 0.5);
    }
  }
  writeImage(pixels, w, h);
}
