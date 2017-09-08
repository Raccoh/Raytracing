#include <random>

#include "Types.h"
#include "Shapes.h"
#include "HelperFunctions.h"

Sphere spheres[] = {
  Sphere(1e5, vec3d(-1e5+1,40.8,81.6),  vec3d(),
         vec3d(.75,.25,.25), DIFF),                       // Left
  Sphere(1e5, vec3d( 1e5+99,40.8,81.6), vec3d(),
         vec3d(.25,.25,.75), DIFF),                       // Right
  Sphere(1e5, vec3d(50,40.8,-1e5),      vec3d(),
         vec3d(.75,.75,.75), DIFF),                       // Back
  Sphere(1e5, vec3d(50,-1e5, 81.6),     vec3d(),
         vec3d(.75,.75,.75), DIFF),                       // Bottom
  Sphere(1e5, vec3d(50, 1e5+81.6,81.6), vec3d(),
         vec3d(.75,.75,.75), DIFF),                       // Top
  Sphere(16.5,vec3d(27, 16.5, 47),      vec3d(),
         vec3d(1.0,1.0,1.0)*0.999, SPEC),                 // Mirror ball
  Sphere(16.5,vec3d(73,16.5,78),        vec3d(),
         vec3d(0.0,1.0,0.0)*0.999, SPEC),                 // Green mirror ball
  Sphere(600, vec3d(50,681.6-.27,81.6), vec3d(12,12,12),
         vec3d(), DIFF)                                   // Area light source
};
int numSpheres = sizeof(spheres) / sizeof(Sphere);

int numSamplesPerPixel = 4;  // if 4, try to read it from command line argument
int maxDepth = 5;
double reflRayOffset = 0.01; // add a bias to refl. rays to avoid false self illumination

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0, 1);

void findIntersection(const Ray& r, int* closestId, double* closestT) {
  for (int i = 0; i < numSpheres; ++i) {
    double t = spheres[i].intersect(r);
    if (t > 0 && t < *closestT) {
      *closestId = i;
      *closestT = t;
    }
  }
}

void createCoordinateSystem(const vec3d& N, vec3d* Nt, vec3d* Nb)
{
  if (std::fabs(N.x) > std::fabs(N.y))
    *Nt = vec3d(N.z, 0, -N.x) * (1 / sqrt(N.x * N.x + N.z * N.z));
  else
    *Nt = vec3d(0, -N.z, N.y) * (1 / sqrt(N.y * N.y + N.z * N.z));
  *Nb = N.cross(*Nt);
}

// Return a sample of a cosine-weighted hemisphere by generating uniform points
// on a disk, and then projecting them up to the hemisphere (and transform with hitNorm)
vec3d cosineSampleHemisphere(const vec3d& hitNorm)
{
  double r1 = distribution(generator);
  double r2 = distribution(generator);
  double r1s = sqrt(r1);
  double phi = 2 * M_PI * r2;
  double x = r1s * cos(phi);
  double z = r1s * sin(phi);
  vec3d sample = vec3d(x, sqrt(1.0 - r1), z).norm();
  vec3d Nt, Nb;
  createCoordinateSystem(hitNorm, &Nt, &Nb);
  vec3d sampleWorld(
      sample.x * Nt.x + sample.y * hitNorm.x + sample.z * Nb.x,
      sample.x * Nt.y + sample.y * hitNorm.y + sample.z * Nb.y,
      sample.x * Nt.z + sample.y * hitNorm.z + sample.z * Nb.z);
  return sampleWorld.norm();
}

// Compute transmitted radiance of follow-up rays (depth > 0)
vec3d radiance(const Ray& r, int depth) {
  int closestId = -1;
  double closestT = Infty;
  findIntersection(r, &closestId, &closestT);
  if (closestId < 0) return vec3d();  // no hit
  vec3d hitPos = r.o + (r.d * closestT);
  if (hitPos.y > 81.7) return vec3d();  // irrelevant hit above the room
  const Sphere& hitObj = spheres[closestId];
  vec3d hitNorm = (hitPos - hitObj.pos).norm();
  if (++depth > maxDepth) return hitObj.emission;  // max number of bounces exceeded
  // Handle diffuse surfaces
  if (hitObj.refl_t == DIFF) {
    // Trace one new random sample ray in the hemisphere at hitPos
    vec3d sample = cosineSampleHemisphere(hitNorm);
    vec3d result = hitObj.emission + (hitObj.color *
                   radiance(Ray(hitPos + (sample * reflRayOffset), sample), depth));
    // Our PDF is now cos(theta)/pi with the cosine-weighted hemisphere.
    // Dividing by that cancels out Lambert's Cosine Law and we just need to
    // multiply all samples by pi. Including the energy conservation constraint
    // 1/pi cancels out that pi as well, so there's nothing left to do here.
    return result;
  }
  // Handle specular surfaces, let the ray bounce off the surface perfectly
  vec3d reflectedDir = r.d - (hitNorm * 2 * hitNorm.dot(r.d));
  return hitObj.emission + (hitObj.color *
         radiance(Ray(hitPos + (reflectedDir * reflRayOffset), reflectedDir), depth));
}

// Compute transmitted radiance of initial rays (depth = 0), sample the complete
// hemisphere if they hit a diffuse surface or bounce off specular surfaces perfectly
vec3d radiance(const Ray& r) {
  int closestId = -1;
  double closestT = Infty;
  findIntersection(r, &closestId, &closestT);
  if (closestId < 0) return vec3d();  // no hit
  vec3d hitPos = r.o + (r.d * closestT);
  if (hitPos.y > 81.7) return vec3d();  // irrelevant hit above the room
  const Sphere& hitObj = spheres[closestId];
  vec3d hitNorm = (hitPos - hitObj.pos).norm();
  // Handle specular surfaces, let the ray bounce off the surface perfectly
  if (hitObj.refl_t == SPEC) {
    vec3d reflectedDir = r.d - (hitNorm * 2 * hitNorm.dot(r.d));
    return hitObj.emission + (hitObj.color *
           radiance(Ray(hitPos + (reflectedDir * reflRayOffset), reflectedDir)));
  }
  // Handle diffuse surfaces, sample the complete hemisphere at hitPos
  vec3d result;
  for (int i = 0; i < numSamplesPerPixel; ++i) {
    vec3d sample = cosineSampleHemisphere(hitNorm);
    result = result + hitObj.emission + (hitObj.color *
             radiance(Ray(hitPos + (sample * reflRayOffset), sample), 1));
  }
  // Only divide by number of samples. Dividing by our PDF cos(theta)/pi
  // canceled out Lambert's Cosine Law earlier and left us with multiplying all
  // samples by pi. Including the energy conservation constraint 1/pi canceled
  // out that pi as well so there's only this operation left to do.
  return result * (1.0 / numSamplesPerPixel);
}

int main(int argc, char* argv[])
{
  // Render the smallpt scene (no glass ball) with Monte Carlo path tracing.
  // Coordinates taken from smallpt.
  int w = 1024, h = 768;
  if (numSamplesPerPixel == 4 && argc == 2) numSamplesPerPixel = atoi(argv[1]);
  if (numSamplesPerPixel < 4) {
    printf("Usage: ./Main <number of samples per pixel, at least 4>\n");
    return 1;
  }
  Ray cam(vec3d(50, 52, 295.6), vec3d(0, -0.042612, -1).norm());
  vec3d cx = vec3d(w*.5135/h, 0, 0), cy = (cx.cross(cam.d)).norm()*.5135;
  vec3d* pixels = new vec3d[w * h];
  #pragma omp parallel for schedule(dynamic, 1)
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
