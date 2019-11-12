#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "Surface.h"
#include "Triangle.h"

class Square : public Surface {
  public:
  Vector3d n;  // Vertex normal.

  // Vertices to be specified in counterclockwise order
  Square(const Vector3d &v0, const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, const Material *mat_ptr) {
    t1 = new Triangle(v0, v1, v3, mat_ptr);
    t2 = new Triangle(v1, v2, v3, mat_ptr);
    n = t1->n0;
    matp = mat_ptr;
  }

  virtual bool hit(
      const Ray &r,  // Ray being sent.
      double tmin,   // Minimum hit parameter to be searched for.
      double tmax,   // Maximum hit parameter to be searched for.
      SurfaceHitRecord &rec) const;

  virtual bool shadowHit(
      const Ray &r,  // Ray being sent.
      double tmin,   // Minimum hit parameter to be searched for.
      double tmax    // Maximum hit parameter to be searched for.
      ) const;

  private:
  Triangle *t1;
  Triangle *t2;
};

#endif  // _SQUARE_H_
