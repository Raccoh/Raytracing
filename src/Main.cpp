#include <random>

#include "Types.h"
#include "Shapes.h"
#include "HelperFunctions.h"

Sphere spheres[] = {
  Sphere(1e5, vec3d(-1e5+1,40.8,81.6),  color4i(),
         color4i(.75,.25,.25), DIFF),                       // Left
  Sphere(1e5, vec3d( 1e5+99,40.8,81.6), color4i(),
         color4i(.25,.25,.75), DIFF),                       // Right
  Sphere(1e5, vec3d(50,40.8,-1e5),      color4i(),
         color4i(.75,.75,.75), DIFF),                       // Back
  Sphere(1e5, vec3d(50,-1e5, 81.6),     color4i(),
         color4i(.75,.75,.75), DIFF),                       // Bottom
  Sphere(1e5, vec3d(50, 1e5+81.6,81.6), color4i(),
         color4i(.75,.75,.75), DIFF),                       // Top
  Sphere(16.5,vec3d(27, 16.5, 47),      color4i(),
         color4i(0.5,0.5,0.5), SPEC),                       // Mirror ball
  Sphere(16.5,vec3d(73,16.5,78),        color4i(),
         color4i(1.0,1.0,1.0), REFR),                       // Glas ball
  Sphere(600, vec3d(50,681.6-.27,81.6), color4i(12,12,12),
         color4i(1.0,1.0,1.0), DIFF)                        // Area light source
};
int numSpheres = sizeof(spheres) / sizeof(Sphere);
int lightId = numSpheres - 1;

int numSamples = 1;  // if 1, try to read it from command line argument
int maxDepth = 1;

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

void createCoordinateSystem(const vec3d& N, vec3d* Nt, vec3d* Nb)
{
  if (std::fabs(N.x) > std::fabs(N.y))
    *Nt = vec3d(N.z, 0, -N.x) * (1 / sqrtf(N.x * N.x + N.z * N.z));
  else
    *Nt = vec3d(0, -N.z, N.y) * (1 / sqrtf(N.y * N.y + N.z * N.z));
  *Nb = N.cross(*Nt);
}

vec3d uniformSampleHemisphere(float r1, float r2)
{
  // cos(theta) = r1 = y
  // cos^2(theta) + sin^2(theta) = 1 -> sin(theta) = srtf(1 - cos^2(theta))
  float sinTheta = sqrtf(1 - r1 * r1);
  float phi = 2 * M_PI * r2;
  float x = sinTheta * cosf(phi);
  float z = sinTheta * sinf(phi);
  return vec3d(x, r1, z).norm();
}

color4i radiance(const Ray& r, int depth = 0, Sphere* s = nullptr) {
  int closestId = -1;
  double closestT = Infty;
  for (int i = 0; i < numSpheres; ++i) {
    double t = spheres[i].intersect(r);
    if (t > 0 && t < closestT) {
      closestId = i;
      closestT = t;
    }
  }
  if (closestId < 0) return color4i();
  vec3d hitPos = r.o + (r.d * closestT);
  if (hitPos.y > 81.7) return color4i();  // irrelevant hit above the room
  vec3d hitNorm = (hitPos - spheres[closestId].pos).norm();
  color4i ambient;
  if (s)
    ambient = spheres[lightId].color * s->color * 0.05;
  if (closestId == lightId) {
    if (!s)
      return spheres[lightId].color;
    return ambient + (spheres[lightId].color * s->color * std::abs(r.d.dot(hitNorm)) * 0.95);
  }
  ++depth;
  if (depth > maxDepth) return ambient;
  vec3d Nt, Nb;
  createCoordinateSystem(hitNorm, &Nt, &Nb);
  int resultR = 0; int resultG = 0; int resultB = 0;
#pragma omp parallel for reduction(+:resultR) reduction(+:resultG) reduction(+:resultB)
  for (int i = 0; i < numSamples; ++i) {
    float r1 = distribution(generator);
    float r2 = distribution(generator);
    vec3d sample = uniformSampleHemisphere(r1, r2);
    vec3d sampleWorld(
        sample.x * Nt.x + sample.y * hitNorm.x + sample.z * Nb.x,
        sample.x * Nt.y + sample.y * hitNorm.y + sample.z * Nb.y,
        sample.x * Nt.z + sample.y * hitNorm.z + sample.z * Nb.z);
    sampleWorld = sampleWorld.norm();
    color4i result = radiance(Ray(hitPos + sampleWorld * 0.1, sampleWorld), depth, &spheres[closestId]);
    resultR += result.r * 15; resultG += result.g * 15; resultB += result.b * 15;
  }
  resultR = (double)resultR / numSamples;
  resultG = (double)resultG / numSamples;
  resultB = (double)resultB / numSamples;
  return ambient + color4i(resultR, resultG, resultB);
}

int main(int argc, char* argv[])
{
  // Render the smallpt scene with simple Monte Carlo path tracing (1 bounce,
  // corresponds to direct lighting with soft shadows).
  // Coordinates taken from smallpt.
  int w = 1024, h = 768;
  if (numSamples == 1 && argc == 2) numSamples = atoi(argv[1]);
  Ray cam(vec3d(50, 52, 295.6), vec3d(0, -0.042612, -1).norm());
  vec3d cx = vec3d(w*.5135/h, 0, 0), cy = (cx.cross(cam.d)).norm()*.5135;
  color4i* pixels = new color4i[w * h];
  for (int row = 0; row < h; ++row) {
    for (int col = 0; col < w; ++col) {
      vec3d d = cx * ((double)col/w - 0.5) + cy * ((double)row/h - 0.5) + cam.d;
      Ray r(cam.o, d.norm());
      int i = (h - row - 1) * w + col;
      pixels[i] = radiance(r);
    }
  }
  writeImage(pixels, w, h);
}
