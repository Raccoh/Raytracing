#include <cstdio>

#include "Types.h"

// Clamp given value to [0,1]
inline double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

// Clamp, apply a gamma correction of 2.2 and return the corresponding 8-bit color value
inline int toInt(double x) { return int(pow(clamp(x), 1 / 2.2) * 255 + 0.5); }

// Write gathered radiance values to image.ppm
void writeImage(vec3d* pixels, int width, int height) {
  FILE* f = fopen("image.ppm", "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      int i = row * width + col;
      fprintf(f, "%d %d %d ", toInt(pixels[i].x),
                              toInt(pixels[i].y),
                              toInt(pixels[i].z));
    }
  }
  fclose(f);
}
