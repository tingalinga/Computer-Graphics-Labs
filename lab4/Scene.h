#ifndef _SCENE_H_
#define _SCENE_H_

#include "Camera.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Surface.h"

struct Scene {
  SurfacePtr *surfacep;  // Array of pointers to surface primitives.
  int numSurfaces;       // Number of surface primitives in array.

  Material *material;  // Array of materials.
  int numMaterials;    // Number of materials in array.

  PointLightSource *ptLight;  // Array of point light sources.
  int numPtLights;            // Number of point light sources in array.

  AmbientLightSource amLight;  // The global ambient light source.

  Color backgroundColor;  // Use this color if ray hits nothing.

  Camera camera;  // The camera.
};

#endif  // _SCENE_H_
