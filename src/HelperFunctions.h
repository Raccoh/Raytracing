#include <cstdio>

#include "Types.h"

void writeImage(vec3d* pixels, int width, int height) {
  FILE* f = fopen("image.ppm", "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      int i = row * width + col;
      fprintf(f, "%d %d %d ", (int)(pixels[i].x * 255 + 0.5),
                              (int)(pixels[i].y * 255 + 0.5),
                              (int)(pixels[i].z * 255 + 0.5));
    }
  }
  fclose(f);
}
