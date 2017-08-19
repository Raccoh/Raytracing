#include <cmath>
#include <cstdio>

struct vec3d {
  double x, y, z;
  vec3d(double x_=0, double y_=0, double z_=0){ x=x_; y=y_; z=z_; }
  vec3d operator+(const vec3d &b) const { return vec3d(x+b.x,y+b.y,z+b.z); }
  vec3d operator-(const vec3d &b) const { return vec3d(x-b.x,y-b.y,z-b.z); }
  vec3d operator*(double b) const { return vec3d(x*b,y*b,z*b); }
  vec3d mult(const vec3d &b) const { return vec3d(x*b.x,y*b.y,z*b.z); }
  vec3d& norm(){ return *this = *this * (1/sqrt(x*x+y*y+z*z)); }
  double dot(const vec3d &b) const { return x*b.x+y*b.y+z*b.z; }
  vec3d operator%(vec3d&b){return vec3d(y*b.z-z*b.y,z*b.x-x*b.z,x*b.y-y*b.x);}
};

void writeImage(vec3d* pixels, int width, int height) {
  FILE* f = fopen("image.ppm", "w");
  fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      int i = row * width + col;
      fprintf(f, "%d %d %d ",
              (int)pixels[i].x, (int)pixels[i].y, (int)pixels[i].z);
    }
  }
  fclose(f);
}

int main()
{
  // Create an image with only blue pixels.
  int w = 1024, h = 768;
  vec3d* pixels = new vec3d[w * h];
  for (int row = 0; row < h; ++row) {
    for (int col = 0; col < w; ++col) {
      pixels[row * w + col] = vec3d(0, 0, 255);
    }
  }
  writeImage(pixels, w, h);
}
