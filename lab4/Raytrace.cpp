//============================================================
// STUDENT NAME: Soeren Hougaard Mulvad
// STUDENT NO.: A0207552J
// NUS EMAIL ADDRESS: e0445646@u.nus.edu
// COMMENTS TO GRADER: None
// ============================================================

#include "Raytrace.h"
#include <cfloat>
#include <cmath>
#include "Color.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Surface.h"
#include "Triangle.h"
#include "Vector3d.h"

using namespace std;

// This is for avoiding the "epsilon problem" or the shadow acne problem.
#define DEFAULT_TMIN 10e-6

// Use this for tmax for non-shadow ray intersection test.
#define DEFAULT_TMAX DBL_MAX

// Compute the outgoing mirror reflection vector.
// Input incoming vector L is pointing AWAY from surface point.
// Assume normal vector N is unit vector.
// The output reflection vector is pointing AWAY from surface point, and
// has same length as incoming vector L.
static Vector3d mirrorReflect(const Vector3d &L, const Vector3d &N) {
  return (2.0 * dot(N, L)) * N - L;
}

// Compute I_source * [ k_d * (N.L) + k_r * (R.V)^n ].
// Input vectors L, N and V are pointing AWAY from surface point.
// Assume all vector L, N and V are unit vectors.
static Color computePhongLighting(const Vector3d &L, const Vector3d &N, const Vector3d &V,
                                  const Material &mat, const PointLightSource &ptLight) {
  Vector3d R = mirrorReflect(L, N);

  float N_dot_L = (float) dot(N, L);
  if (N_dot_L < 0.0f) N_dot_L = 0.0f;

  float R_dot_V = (float) dot(R, V);
  if (R_dot_V < 0.0f) R_dot_V = 0.0f;
  float R_dot_V_pow_n = powf(R_dot_V, (float) mat.n);

  return ptLight.I_source * (mat.k_d * N_dot_L + mat.k_r * R_dot_V_pow_n);
}

// Returns the index that contains the biggest absolute value in a direction vector
// This is to minimize floating point error for the shadow caster
// I.e [-20.0, 0.0000001, -3.1] -> 0
//     [0.000234, -0.0041, 0.5] -> 2
int getMaxIdx(Vector3d dir) {
  double maxIdx, dirAbsMax = -1;
  for (int i = 0; i <= 2; i++) {
    double absDirI = abs(dir[i]);
    if (absDirI > dirAbsMax) {
      dirAbsMax = absDirI;
      maxIdx = i;
    }
  }
  return maxIdx;
}

// Traces a ray into the scene.
// reflectLevels: specfies number of levels of reflections (0 for no reflection).
// hasShadow: specifies whether to generate shadows.
Color Raytrace::TraceRay(const Ray &ray, const Scene &scene,
                         int reflectLevels, bool hasShadow) {
  Ray uRay(ray);
  uRay.makeUnitDirection();  // Normalize ray direction.

  // Find whether and where the ray hits some surface.
  // Take the nearest hit point.

  bool hasHitSomething = false;
  double nearest_t = DEFAULT_TMAX;
  SurfaceHitRecord nearestHitRec;

  for (int i = 0; i < scene.numSurfaces; i++) {
    SurfaceHitRecord tempHitRec;
    bool hasHit = scene.surfacep[i]->hit(uRay, DEFAULT_TMIN, DEFAULT_TMAX, tempHitRec);

    if (hasHit && tempHitRec.t < nearest_t) {
      hasHitSomething = true;
      nearest_t = tempHitRec.t;
      nearestHitRec = tempHitRec;
    }
  }

  if (!hasHitSomething) return scene.backgroundColor;

  nearestHitRec.normal.makeUnitVector();
  Vector3d N = nearestHitRec.normal;  // Unit vector.
  Vector3d V = -uRay.direction();     // Unit vector.

  Color result(0.0f, 0.0f, 0.0f);  // The result will be accumulated here.

  // Add to result the phong lighting contributed by each point light source.
  // Compute for shadow if hasShadow is true.

  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  for (int i = 0; i < scene.numPtLights; i++) {
    // Make unit vector L pointing from hit position p to light point
    Vector3d L = scene.ptLight[i].position - nearestHitRec.p;
    L.makeUnitVector();

    // Don't add lighting if light ray hits some surface before hitting light
    bool lightReached = true;
    if (hasShadow) {
      Ray lRay = Ray(nearestHitRec.p, L);

      // Calculate the maximum t-value using the fact that:
      // lRay.origin() + tmax * lRay.direction() = scene.ptLight[i].position
      // We can in principle do this for any single xyz-axis, but choose the
      // one with the biggest absolute value as to minimize floating point error
      Vector3d dir = lRay.direction();
      double maxIdx = getMaxIdx(dir);
      double tmax = (scene.ptLight[i].position[maxIdx] - lRay.origin()[maxIdx]) / dir[maxIdx];

      for (int i = 0; i < scene.numSurfaces; i++) {
        bool shadowHit = scene.surfacep[i]->shadowHit(lRay, DEFAULT_TMIN, tmax);
        if (shadowHit) {
          lightReached = false;
          break;
        }
      }
    }

    // If we didn't set lightReached to false in previous for-loop, meaning
    // our light ray didn't hit any object inbetween, then add lighting
    if (lightReached)
      result += computePhongLighting(L, N, V, *nearestHitRec.mat_ptr, scene.ptLight[i]);
  }

  // Add to result the global ambient lighting.
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  result += scene.amLight.I_a * nearestHitRec.mat_ptr->k_a;

  // Add to result the reflection of the scene.
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************

  // Construct a ray starting at where we hitted some material and let it's
  // direction be pointing towards the mirrored direction.
  if (reflectLevels > 0) {
    Vector3d R = mirrorReflect(V, N);
    Ray rRay = Ray(nearestHitRec.p, R);
    rRay.makeUnitDirection();
    Color reflected = TraceRay(rRay, scene, reflectLevels - 1, hasShadow);
    result += nearestHitRec.mat_ptr->k_rg * reflected;
  }

  return result;
}
