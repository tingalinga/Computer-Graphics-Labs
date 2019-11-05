//============================================================
// STUDENT NAME: Soeren Hougaard Mulvad
// STUDENT NO.: A0207552J
// NUS EMAIL ADDRESS: e0445646@u.nus.edu
// COMMENTS TO GRADER: None
// ============================================================

#include "Sphere.h"
#include <cmath>

using namespace std;

bool Sphere::hit(const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec) const {
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  Vector3d oc = r.origin() - center;
  double a = 1.0;  // dot(r.direction, r.direction) which we know is 1
  double b = 2.0 * dot(r.direction(), oc);
  double c = dot(oc, oc) - radius * radius;
  double d = b * b - 4 * a * c;

  if (d < 0.0) return false;  // No real solutions
  double dSqrt = sqrt(d);
  double t1 = (-b - dSqrt) / (2.0 * a);
  double t2 = (-b + dSqrt) / (2.0 * a);

  // Use the closet (smallest) positive (or zero) t value
  // NB: This doesn't take into the account if we were inside the sphere, but not
  // needed for the computations we want to perform.
  bool t1ClosestAndPos = t1 >= 0.0 && (t1 <= t2 || t2 < 0.0);
  bool t2ClosestAndPos = t2 >= 0.0 && (t2 <= t1 || t1 < 0.0);

  // If both are negative it means the ray is pointing away from the sphere
  if (!(t1ClosestAndPos || t2ClosestAndPos)) return false;

  double tVal = t1ClosestAndPos ? t1 : t2;
  if (tVal < tmin || tVal > tmax) return false;
  rec.t = tVal;
  rec.p = r.pointAtParam(tVal);
  rec.normal = rec.p.unitVector();
  rec.mat_ptr = matp;
  return true;
}

bool Sphere::shadowHit(const Ray &r, double tmin, double tmax) const {
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  Vector3d oc = r.origin() - center;
  double a = 1.0;
  double b = 2.0 * dot(r.direction(), oc);
  double c = dot(oc, oc) - radius * radius;
  double d = b * b - 4 * a * c;

  if (d < 0.0) return false;

  double dSqrt = sqrt(d);
  double t1 = (-b - dSqrt) / (2.0 * a);
  double t2 = (-b + dSqrt) / (2.0 * a);

  bool t1ClosestAndPos = (t1 >= 0.0 && (t1 <= t2 || t2 < 0.0));
  bool t2ClosestAndPos = (t2 >= 0.0 && (t2 <= t1 || t1 < 0.0));

  return ((t1ClosestAndPos && t1 >= tmin && t1 <= tmax) ||
          (t2ClosestAndPos && t2 >= tmin && t2 <= tmax));
}
