#include "Square.h"
#include <cmath>
#include "Triangle.h"

using namespace std;

bool Square::hit(const Ray &r, double tmin, double tmax, SurfaceHitRecord &rec) const {
  bool t1Hit = t1->hit(r, tmin, tmax, rec);
  // If it hit t1, return true right away. Otherwise check if it hits t2 and return this value
  return t1Hit ? true : t2->hit(r, tmin, tmax, rec);
}

bool Square::shadowHit(const Ray &r, double tmin, double tmax) const {
  return (t1->shadowHit(r, tmin, tmax) || t2->shadowHit(r, tmin, tmax));
}
