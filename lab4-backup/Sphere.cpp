//============================================================
// STUDENT NAME:
// STUDENT NO.:
// NUS EMAIL ADDRESS:
// COMMENTS TO GRADER:
//
// ============================================================

#include <cmath>
#include "Sphere.h"

using namespace std;

// A simple expression for silencing compiler warnings for
// unused parameters without altering program flow
#ifndef UNUSED
#define UNUSED(expr) \
  do {               \
    (void) (expr);   \
  } while (0)
#define UNUNSED_VAR 0
#endif



bool Sphere::hit(const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec) const {
    UNUSED(r);
    UNUSED(tmin);
    UNUSED(tmax);
    UNUSED(rec);
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    return false; // You can remove/change this if needed.
}



bool Sphere::shadowHit(const Ray &r, double tmin, double tmax) const {
    UNUSED(r);
    UNUSED(tmin);
    UNUSED(tmax);
    //***********************************************
    //*********** WRITE YOUR CODE HERE **************
    //***********************************************

    return false; // You can remove/change this if needed.
}
