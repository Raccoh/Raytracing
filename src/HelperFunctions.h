#include <cstdio>

#include "Types.h"

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
