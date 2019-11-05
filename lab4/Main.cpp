//============================================================
// STUDENT NAME: Soeren Hougaard Mulvad
// STUDENT NO.: A0207552J
// NUS EMAIL ADDRESS: e0445646@u.nus.edu
// COMMENTS TO GRADER: This program has been developed and tested on macOS
// and compiled with:
// g++ -I./include/ -Wno-deprecated -c image_io.cpp
// g++ -lGLEW -framework GLUT -framework OpenGL -Wno-deprecated -o main main.cpp image_io.o
// There may therefore be problems with library functions, although
// I believe there shouldn't.
// ============================================================

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "Camera.h"
#include "Color.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Plane.h"
#include "Ray.h"
#include "Raytrace.h"
#include "Scene.h"
#include "Sphere.h"
#include "Surface.h"
#include "Triangle.h"
#include "Util.h"
#include "Vector3d.h"

using namespace std;

// Constants for Scene 1.
static const int imageHeight1 = 480;
static const int imageWidth1 = 640;

// TODO: Maybe comment these in again
// static const int reflectLevels1 = 2;  // 0 -- object does not reflect scene.
// static const int hasShadow1 = true;
// static const char outImageFile1[] = "out1.png";

// Constants for Scene 2.
static const int imageWidth2 = 640;
static const int imageHeight2 = 480;
static const int reflectLevels2 = 2;  // 0 -- object does not reflect scene.
static const int hasShadow2 = true;
static const char outImageFile2[] = "img_scene2.png";

// Raytrace the whole image of the scene and write it to a file.
void RenderImage(const char *imageFilename,
                 const Scene &scene,
                 int reflectLevels,
                 bool hasShadow) {
  int imgWidth = scene.camera.getImageWidth();
  int imgHeight = scene.camera.getImageHeight();

  Image image(imgWidth, imgHeight);  // To store the result of ray tracing.

  double startTime = Util::GetCurrRealTime();
  double startCPUTime = Util::GetCurrCPUTime();

  // Generate image.
  for (int y = 0; y < imgHeight; y++) {
    double pixelPosY = y + 0.5;

    for (int x = 0; x < imgWidth; x++) {
      double pixelPosX = x + 0.5;
      Ray ray = scene.camera.getRay(pixelPosX, pixelPosY);
      Color pixelColor = Raytrace::TraceRay(ray, scene, reflectLevels, hasShadow);
      pixelColor.clamp();
      image.setPixel(x, y, pixelColor);
    }
    // printf( "%d ", y );
  }

  double stopCPUTime = Util::GetCurrCPUTime();
  double stopTime = Util::GetCurrRealTime();
  printf("CPU time taken  = %.1f sec\n", stopCPUTime - startCPUTime);
  printf("Real time taken = %.1f sec\n", stopTime - startTime);

  // Write image to file.
  image.writeToFile(imageFilename);
}

// Forward declarations. These functions are defined later in the file.
void DefineScene1(Scene &scene, int imageWidth, int imageHeight);
void DefineScene2(Scene &scene, int imageWidth, int imageHeight);

void WaitForEnterKeyBeforeExit(void) {
  fflush(stdin);
  getchar();
}

int main() {
  // TODO: Comment this in again
  // atexit(WaitForEnterKeyBeforeExit);

  // Define Scene 1.
  if (false) {  // TODO: Comment this in again
    Scene scene1;
    DefineScene1(scene1, imageWidth1, imageHeight1);

    // Render Scene 1.
    for (int r = 0; r <= 2; r++) {    // reflectLevels [0, 1, 2]
      for (int h = 0; h <= 1; h++) {  // hasShadow [0, 1] = [false, true]
        // Construct output filename
        string outputFile = "img_r" + to_string(r) + (h ? "s" : "") + ".png";
        // Make C++ string to a char[]
        char charArr[outputFile.length() + 1];
        strcpy(charArr, outputFile.c_str());

        printf("Render Scene 1 with %i reflection levels and %sshadows...\n",
               r, h ? "" : "no ");
        RenderImage(charArr, scene1, r, h);
        printf("Image completed.\n\n");
      }
    }
  }

  // TODO: Maybe remove this.
  // printf("Render Scene 1...\n");
  // RenderImage(outImageFile1, scene1, reflectLevels1, hasShadow1);
  // printf("Image completed.\n");

  // Define Scene 2.
  Scene scene2;
  DefineScene2(scene2, imageWidth2, imageHeight2);

  // Render Scene 2.
  printf("Render Scene 2...\n");
  RenderImage(outImageFile2, scene2, reflectLevels2, hasShadow2);
  printf("Image completed.\n");

  printf("All done.\n");
  return 0;
}

// Modeling of Scene 1.
void DefineScene1(Scene &scene, int imageWidth, int imageHeight) {
  scene.backgroundColor = Color(0.2f, 0.3f, 0.5f);

  scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

  // Define materials.
  scene.numMaterials = 5;
  scene.material = new Material[scene.numMaterials];

  // Light red.
  scene.material[0].k_d = Color(0.8f, 0.4f, 0.4f);
  scene.material[0].k_a = scene.material[0].k_d;
  scene.material[0].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  scene.material[0].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  scene.material[0].n = 64.0f;

  // Light green.
  scene.material[1].k_d = Color(0.4f, 0.8f, 0.4f);
  scene.material[1].k_a = scene.material[0].k_d;
  scene.material[1].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  scene.material[1].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  scene.material[1].n = 64.0f;

  // Light blue.
  scene.material[2].k_d = Color(0.4f, 0.4f, 0.8f) * 0.9f;
  scene.material[2].k_a = scene.material[0].k_d;
  scene.material[2].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  scene.material[2].k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
  scene.material[2].n = 64.0f;

  // Yellow.
  scene.material[3].k_d = Color(0.6f, 0.6f, 0.2f);
  scene.material[3].k_a = scene.material[0].k_d;
  scene.material[3].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  scene.material[3].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  scene.material[3].n = 64.0f;

  // Gray.
  scene.material[4].k_d = Color(0.6f, 0.6f, 0.6f);
  scene.material[4].k_a = scene.material[0].k_d;
  scene.material[4].k_r = Color(0.6f, 0.6f, 0.6f);
  scene.material[4].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  scene.material[4].n = 128.0f;

  // Define point light sources.
  scene.numPtLights = 2;
  scene.ptLight = new PointLightSource[scene.numPtLights];

  scene.ptLight[0].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
  scene.ptLight[0].position = Vector3d(100.0, 120.0, 10.0);

  scene.ptLight[1].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
  scene.ptLight[1].position = Vector3d(5.0, 80.0, 60.0);

  // Define surface primitives.
  scene.numSurfaces = 15;
  scene.surfacep = new SurfacePtr[scene.numSurfaces];

  scene.surfacep[0] = new Plane(0.0, 1.0, 0.0, 0.0, &(scene.material[2]));                 // Horizontal plane.
  scene.surfacep[1] = new Plane(1.0, 0.0, 0.0, 0.0, &(scene.material[4]));                 // Left vertical plane.
  scene.surfacep[2] = new Plane(0.0, 0.0, 1.0, 0.0, &(scene.material[4]));                 // Right vertical plane.
  scene.surfacep[3] = new Sphere(Vector3d(40.0, 20.0, 42.0), 22.0, &(scene.material[0]));  // Big sphere.
  scene.surfacep[4] = new Sphere(Vector3d(75.0, 10.0, 40.0), 12.0, &(scene.material[1]));  // Small sphere.

  // Cube +y face.
  scene.surfacep[5] = new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(50.0, 20.0, 70.0),
                                   Vector3d(30.0, 20.0, 70.0), &(scene.material[3]));
  scene.surfacep[6] = new Triangle(Vector3d(50.0, 20.0, 90.0), Vector3d(30.0, 20.0, 70.0),
                                   Vector3d(30.0, 20.0, 90.0), &(scene.material[3]));

  // Cube +x face.
  scene.surfacep[7] = new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
                                   Vector3d(50.0, 20.0, 90.0), &(scene.material[3]));
  scene.surfacep[8] = new Triangle(Vector3d(50.0, 0.0, 70.0), Vector3d(50.0, 20.0, 90.0),
                                   Vector3d(50.0, 0.0, 90.0), &(scene.material[3]));

  // Cube -x face.
  scene.surfacep[9] = new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
                                   Vector3d(30.0, 20.0, 70.0), &(scene.material[3]));
  scene.surfacep[10] = new Triangle(Vector3d(30.0, 0.0, 90.0), Vector3d(30.0, 20.0, 70.0),
                                    Vector3d(30.0, 0.0, 70.0), &(scene.material[3]));

  // Cube +z face.
  scene.surfacep[11] = new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(50.0, 20.0, 90.0),
                                    Vector3d(30.0, 20.0, 90.0), &(scene.material[3]));
  scene.surfacep[12] = new Triangle(Vector3d(50.0, 0.0, 90.0), Vector3d(30.0, 20.0, 90.0),
                                    Vector3d(30.0, 0.0, 90.0), &(scene.material[3]));

  // Cube -z face.
  scene.surfacep[13] = new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(30.0, 20.0, 70.0),
                                    Vector3d(50.0, 20.0, 70.0), &(scene.material[3]));
  scene.surfacep[14] = new Triangle(Vector3d(30.0, 0.0, 70.0), Vector3d(50.0, 20.0, 70.0),
                                    Vector3d(50.0, 0.0, 70.0), &(scene.material[3]));

  // Define camera.
  scene.camera = Camera(Vector3d(150.0, 120.0, 150.0), Vector3d(45.0, 22.0, 55.0), Vector3d(0.0, 1.0, 0.0),
                        (-1.0 * imageWidth) / imageHeight, (1.0 * imageWidth) / imageHeight, -1.0, 1.0, 3.0,
                        imageWidth, imageHeight);
}

// Modeling of Scene 2
void DefineScene2(Scene &scene, int imageWidth, int imageHeight) {
  //***********************************************
  //*********** WRITE YOUR CODE HERE **************
  //***********************************************
  scene.backgroundColor = Color(0.0f, 0.0f, 1.0f);

  scene.amLight.I_a = Color(1.0f, 1.0f, 1.0f) * 0.25f;

  // Define materials.
  scene.numMaterials = 5;
  scene.material = new Material[scene.numMaterials];

  // Very reflective dark blue.
  scene.material[0].k_d = Color(0.2f, 0.2f, 0.9f);
  scene.material[0].k_a = scene.material[0].k_d;
  scene.material[0].k_r = Color(0.9f, 0.9f, 0.9f);
  scene.material[0].k_rg = Color(0.8f, 0.8f, 0.8f) / 1.4f;
  scene.material[0].n = 30.0f;

  // Dark green
  scene.material[1].k_d = Color(0.3f, 1.0f, 0.3f);
  scene.material[1].k_a = scene.material[1].k_d;
  scene.material[1].k_r = Color(0.2f, 0.4f, 0.2f);
  scene.material[1].k_rg = Color(0.8f, 0.8f, 0.8f) / 4.0f;
  scene.material[1].n = 30.0f;

  // Light blue.
  scene.material[2].k_d = Color(0.3f, 0.3f, 0.8f);
  scene.material[2].k_a = scene.material[2].k_d;
  scene.material[2].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  scene.material[2].k_rg = Color(0.8f, 0.8f, 0.8f) / 2.5f;
  scene.material[2].n = 64.0f;

  // Light red.
  scene.material[3].k_d = Color(0.8f, 0.4f, 0.4f);
  scene.material[3].k_a = scene.material[3].k_d;
  scene.material[3].k_r = Color(0.8f, 0.8f, 0.8f) / 1.5f;
  scene.material[3].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  scene.material[3].n = 64.0f;

  // Gray.
  scene.material[4].k_d = Color(0.6f, 0.6f, 0.6f);
  scene.material[4].k_a = scene.material[4].k_d;
  scene.material[4].k_r = Color(0.6f, 0.6f, 0.6f);
  scene.material[4].k_rg = Color(0.8f, 0.8f, 0.8f) / 3.0f;
  scene.material[4].n = 128.0f;

  // Define point light sources.
  scene.numPtLights = 2;
  scene.ptLight = new PointLightSource[scene.numPtLights];

  scene.ptLight[0].I_source = Color(1.0f, 1.0f, 1.0f) * 0.4f;
  scene.ptLight[0].position = Vector3d(100.0, 120.0, 10.0);

  scene.ptLight[1].I_source = Color(1.0f, 1.0f, 1.0f) * 0.6f;
  scene.ptLight[1].position = Vector3d(5.0, 80.0, 60.0);

  // Define surface primitives.
  scene.numSurfaces = 19;
  scene.surfacep = new SurfacePtr[scene.numSurfaces];

  scene.surfacep[0] = new Plane(0.0, 1.0, 0.0, 0.0, &(scene.material[2]));  // Horizontal plane.
  scene.surfacep[1] = new Plane(1.0, 0.0, 0.0, 0.0, &(scene.material[4]));  // Left vertical plane.
  scene.surfacep[2] = new Plane(0.0, 0.0, 1.0, 0.0, &(scene.material[4]));  // Right vertical plane.

  // Constants related to cube and sphere on top of cube
  const double C_SPHERE_X = 60.0;
  const double C_SPHERE_Z = 30.0;
  const double C_HEIGHT = 15.0;
  const double C_RADIUS = 10.0;

  const double C_START_X = 0.5 * C_SPHERE_X;
  const double C_START_Z = C_SPHERE_Z - 1.5 * C_RADIUS;
  const double C_END_X = 1.5 * C_SPHERE_X;
  const double C_END_Z = C_SPHERE_Z + 1.5 * C_RADIUS;

  // Sphere on top of cube
  scene.surfacep[3] = new Sphere(Vector3d(C_SPHERE_X, C_HEIGHT + C_RADIUS, C_SPHERE_Z), C_RADIUS, &(scene.material[0]));

  // Vertices defining a cube
  Vector3d C_A = Vector3d(C_START_X, C_HEIGHT, C_END_Z);
  Vector3d C_B = Vector3d(C_START_X, C_HEIGHT, C_START_Z);
  Vector3d C_C = Vector3d(C_END_X, C_HEIGHT, C_START_Z);
  Vector3d C_D = Vector3d(C_END_X, C_HEIGHT, C_END_Z);
  Vector3d C_E = Vector3d(C_END_X, 0.0, C_START_Z);
  Vector3d C_F = Vector3d(C_END_X, 0.0, C_END_Z);
  Vector3d C_G = Vector3d(C_START_X, 0.0, C_END_Z);
  Vector3d C_H = Vector3d(C_START_X, 0.0, C_START_Z);

  // Cube +y face.
  scene.surfacep[4] = new Triangle(C_D, C_B, C_A, &(scene.material[3]));
  scene.surfacep[5] = new Triangle(C_D, C_C, C_B, &(scene.material[3]));

  // Cube +x face.
  scene.surfacep[6] = new Triangle(C_F, C_C, C_D, &(scene.material[3]));
  scene.surfacep[7] = new Triangle(C_F, C_E, C_C, &(scene.material[3]));

  // Cube -x face
  scene.surfacep[8] = new Triangle(C_A, C_H, C_G, &(scene.material[3]));
  scene.surfacep[9] = new Triangle(C_A, C_B, C_H, &(scene.material[3]));

  // Cube +z face
  scene.surfacep[10] = new Triangle(C_G, C_D, C_A, &(scene.material[3]));
  scene.surfacep[11] = new Triangle(C_G, C_F, C_D, &(scene.material[3]));

  // Cube -z face
  scene.surfacep[12] = new Triangle(C_B, C_C, C_E, &(scene.material[3]));
  scene.surfacep[13] = new Triangle(C_B, C_E, C_H, &(scene.material[3]));

  const double P_HEIGHT = 50.0;
  const double P_WIDTH = 25.0;

  const double P_START_X = C_SPHERE_X - 0.5 * P_WIDTH;
  const double P_START_Z = C_END_Z + 40.0;
  const double P_END_X = P_START_X + P_WIDTH;
  const double P_END_Z = P_START_Z + P_WIDTH;

  const double P_MIDDLE_X = P_START_X + 0.5 * P_WIDTH;
  const double P_MIDDLE_Z = P_START_Z + 0.5 * P_WIDTH;

  // Vertices defining pyramid
  Vector3d P_A = Vector3d(P_START_X, 0.0, P_START_Z);
  Vector3d P_B = Vector3d(P_START_X, 0.0, P_END_Z);
  Vector3d P_C = Vector3d(P_END_X, 0.0, P_END_Z);
  Vector3d P_D = Vector3d(P_END_X, 0.0, P_START_Z);
  Vector3d P_E = Vector3d(P_MIDDLE_X, P_HEIGHT, P_MIDDLE_Z);

  // Pyramid +x
  scene.surfacep[14] = new Triangle(P_D, P_E, P_C, &(scene.material[1]));
  // Pyramid -x
  scene.surfacep[15] = new Triangle(P_A, P_B, P_E, &(scene.material[1]));
  // Pyramid +z
  scene.surfacep[16] = new Triangle(P_C, P_E, P_B, &(scene.material[1]));
  // Pyramid -z
  scene.surfacep[17] = new Triangle(P_A, P_D, P_E, &(scene.material[1]));

  // Sphere on top of pyramid
  const double P_RADIUS = 0.8 * C_RADIUS;
  scene.surfacep[18] = new Sphere(Vector3d(P_MIDDLE_X, P_HEIGHT + P_RADIUS - 10.0, P_MIDDLE_Z), P_RADIUS, &(scene.material[1]));

  // Define camera.
  scene.camera = Camera(Vector3d(200.0, 50.0, 140.0),       // Eye
                        Vector3d(45.0, 22.0, 55.0),         // LookAt
                        Vector3d(0.0, 1.0, 0.0),            // Up vector
                        (-1.0 * imageWidth) / imageHeight,  // Left
                        (1.0 * imageWidth) / imageHeight,   // Right
                        -1.0, 1.0,                          // Bottom / Top
                        3.0,                                // Near
                        imageWidth, imageHeight);
}
