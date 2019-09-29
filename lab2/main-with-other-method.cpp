//============================================================
// STUDENT NAME: Soeren Hougaard Mulvad
// STUDENT NO.: A0207552J
// NUS EMAIL ADDRESS: e0445646@u.nus.edu
// COMMENTS TO GRADER: This program has been developed and tested on macOS
// and compiled with: "gcc main.cpp -o main -framework GLUT -framework OpenGL
// -Wall -Wextra -Wno-deprecated"
// There may therefore be problems with library functions, although
// I believe there shouldn't.
// ============================================================

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433832795

#define NUM_CARS 12  // Total number of cars.
#define PLANET_RADIUS 100.0

#define CAR_LENGTH 32.0
#define CAR_WIDTH 16.0
#define CAR_HEIGHT 14.0

#define CAR_TOP_LENGTH (CAR_LENGTH * 0.6)  // Length of top part of car
#define CAR_TOP_HEIGHT (CAR_HEIGHT * 0.6)  // Height of top part of car
#define CAR_BOTTOM_HEIGHT (CAR_HEIGHT - CAR_TOP_HEIGHT)

#define TYRE_OUTER_R (0.5 * CAR_BOTTOM_HEIGHT)
#define TYRE_INNER_R (0.4 * TYRE_OUTER_R)
#define TYRE_XDIST_FROM_CAR_C (0.3 * CAR_LENGTH)
// Distance from centrum of car to centrum of tyre so the edges just touch
#define TYRE_YDIST_FROM_CAR_C (0.5 * (CAR_WIDTH + TYRE_OUTER_R))

#define CAR_MIN_ANGLE_INCR 0.5                     // Min degrees to rotate car around planet each frame.
#define CAR_MAX_ANGLE_INCR 3.0                     // Max degrees to rotate car around planet each frame.
#define CAR_TOP_DIST (PLANET_RADIUS + CAR_HEIGHT)  // Distance of the top of a car from planet's center.

#define EYE_INIT_DIST (3.0 * CAR_TOP_DIST)  // Initial distance of eye from planet's center.
#define EYE_DIST_INCR (0.1 * CAR_TOP_DIST)  // Distance increment when changing eye's distance.
#define EYE_MIN_DIST (1.5 * CAR_TOP_DIST)   // Min eye's distance from planet's center.

#define EYE_MIN_LATITUDE -85.0  // Min eye's latitude (in degrees).
#define EYE_MAX_LATITUDE 85.0   // Max eye's latitude (in degrees).
#define EYE_LATITUDE_INCR 2.0   // Degree increment when changing eye's latitude.
#define EYE_LONGITUDE_INCR 2.0  // Degree increment when changing eye's longitude.

#define VERT_FOV 45.0                         // Vertical FOV (in degrees) of the perspective camera.
#define CLIP_PLANE_DIST (1.1 * CAR_TOP_DIST)  // Distance of near or far clipping plane from planet's center.

#define DESIRED_FPS 60  // Approximate desired number of frames per second.
#define MS_BETWEEN_FRAMES (unsigned int) 1000 / DESIRED_FPS

// Trig functions that takes degrees instead of radians as argument
#define sinDeg(x) sin((x) *PI / 180.0)
#define cosDeg(x) cos((x) *PI / 180.0)

// A simple expression for silencing compiler warnings for
// unused parameters without altering program flow
#define UNUSED(expr) \
  do {               \
    (void) (expr);   \
  } while (0)
#define UNUNSED_VAR 0

// All the four different combinations of sign values for (x, y) coordinates
#define NUM_SIGNS 4
const int SIGNS[NUM_SIGNS][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

// Planet's color.
const GLfloat planetColor[] = {0.9, 0.6, 0.4};

// Car tyre color.
const GLfloat tyreColor[] = {0.2, 0.2, 0.2};

// Material properties for all objects.
const GLfloat materialSpecular[] = {1.0, 1.0, 1.0, 1.0};
const GLfloat materialShininess[] = {100.0};
const GLfloat materialEmission[] = {0.0, 0.0, 0.0, 1.0};

// Light 0.
const GLfloat light0Ambient[] = {0.1, 0.1, 0.1, 1.0};
const GLfloat light0Diffuse[] = {0.7, 0.7, 0.7, 1.0};
const GLfloat light0Specular[] = {0.9, 0.9, 0.9, 1.0};
const GLfloat light0Position[] = {1.0, 1.0, 1.0, 0.0};

// Light 1.
const GLfloat light1Ambient[] = {0.1, 0.1, 0.1, 1.0};
const GLfloat light1Diffuse[] = {0.7, 0.7, 0.7, 1.0};
const GLfloat light1Specular[] = {0.9, 0.9, 0.9, 1.0};
const GLfloat light1Position[] = {-1.0, 0.0, -0.5, 0.0};

/////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

// Define the cars.
typedef struct CarType {
  float bodyColor[3];  // RGB color of the car body.
  double angleIncr;    // Degrees to rotate car around planet each frame.
  double angularPos;   // Angular position of car around planet (in degrees).
  double xzAxis[2];    // A vector in the x-z plane. Contains the x and z components respectively.
  double rotAngle;     // Rotation angle about the xzAxis[].
} CarType;

CarType car[NUM_CARS];  // Array of cars.

// Define eye position.
// Initial eye position is at [ 0, 0, EYE_INIT_DIST ] in the world frame,
// looking at the world origin.
// The up-vector is assumed to be [0, 1, 0].
double eyeLatitude = 0;
double eyeLongitude = 0;
double eyeDistance = EYE_INIT_DIST;

// Window's size.
int winWidth = 800;   // Window width in pixels.
int winHeight = 600;  // Window height in pixels.

// Others.
bool showAnimation = true;   // Freeze the cars iff true.
bool drawAxes = true;        // Draw world coordinate frame axes iff true.
bool drawWireframe = false;  // Draw polygons in wireframe if true, otherwise polygons are filled.

/////////////////////////////////////////////////////////////////////////////
// Draw a car with its bottom on the z = 0 plane. The car is heading in the
// +x direction and its top facing the +z direction.
// The z-axis passes through the center of the car.
// The car body is drawn using the input color, and its tyres are drawn
// using the constant tyreColor.
// The car has size CAR_LENGTH x CAR_WIDTH x CAR_HEIGHT.
/////////////////////////////////////////////////////////////////////////////
void DrawOneCar(float bodyColor[3]) {
  // clang-format off

  // Draw bottom part of car
  glColor3fv(bodyColor);
  glPushMatrix();
    // Translate to lie on z = 0 plane
    glTranslatef(0.0, 0.0, 0.5 * CAR_BOTTOM_HEIGHT);
    // Scale cube to size of bottom part of car
    glScalef(CAR_LENGTH, CAR_WIDTH, CAR_BOTTOM_HEIGHT);
    glutSolidCube(1);
  glPopMatrix();

  // Draw top part of car
  glPushMatrix();
    // Translate to lie on "z = 0 + top of bottom part"-plane
    glTranslatef(0.0, 0.0, 0.5 * CAR_TOP_HEIGHT + CAR_BOTTOM_HEIGHT);
    // Scale cube to size of top part of car
    glScalef(CAR_TOP_LENGTH, CAR_WIDTH, CAR_TOP_HEIGHT);
    glutSolidCube(1);
  glPopMatrix();

  // Draw the four tyres, looping though all four possible combinations of
  // sign values for x and y
  glColor3fv(tyreColor);
  for (int i = 0; i < NUM_SIGNS; i++) {
    glPushMatrix();
      // Translate tyre to be placed on side of car ((x, y)-values)
      // and move up to lie on z = 0 plane (z-value)
      glTranslated(SIGNS[i][0] * TYRE_XDIST_FROM_CAR_C,
                   SIGNS[i][1] * TYRE_YDIST_FROM_CAR_C,
                   TYRE_OUTER_R + TYRE_INNER_R);
      // Rotate tyre so the y-axis goes directly through tyre's hole
      glRotated(90.0, 1.0, 0.0, 0.0);
      glutSolidTorus(TYRE_INNER_R, TYRE_OUTER_R, 30, 30);
    glPopMatrix();
  }
  // clang-format on
}

/////////////////////////////////////////////////////////////////////////////
// Draw all the cars. Each is put correctly on its great circle.
/////////////////////////////////////////////////////////////////////////////
void DrawAllCars(void) {
  for (int i = 0; i < NUM_CARS; i++) {
    CarType& curCar = car[i];

    // clang-format off
    glPushMatrix();
      // Rotate great circle in y = 0 plane to current cars great circle
      glRotated(curCar.rotAngle, curCar.xzAxis[0], 0.0, curCar.xzAxis[1]);
      // Rotate around y so we get great circle around y = 0 plane
      glRotated(curCar.angularPos, 0.0, 1.0, 0.0);
      // Translate out to a distance of PLANET_RADIUS in y = 0 plane
      glTranslated(0.0, 0.0, PLANET_RADIUS);
      DrawOneCar(curCar.bodyColor);
    glPopMatrix();
    // clang-format on
  }
}

/////////////////////////////////////////////////////////////////////////////
// Draw the x, y, z axes. Each is drawn with the input length.
// The x-axis is red, y-axis green, and z-axis blue.
/////////////////////////////////////////////////////////////////////////////
void DrawAxes(double length) {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_LIGHTING);
  glLineWidth(3.0);
  glBegin(GL_LINES);
  // x-axis.
  glColor3f(1.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(length, 0.0, 0.0);
  // y-axis.
  glColor3f(0.0, 1.0, 0.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, length, 0.0);
  // z-axis.
  glColor3f(0.0, 0.0, 1.0);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, length);
  glEnd();
  glPopAttrib();
}

/////////////////////////////////////////////////////////////////////////////
// The display callback function.
/////////////////////////////////////////////////////////////////////////////
void MyDisplay(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(VERT_FOV,
                 (double) winWidth / winHeight,
                 eyeDistance - CLIP_PLANE_DIST,
                 eyeDistance + CLIP_PLANE_DIST);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Calculate the eye-coordinates based on current longitude and latitude
  double xEye = eyeDistance * cosDeg(eyeLatitude) * sinDeg(eyeLongitude);
  double yEye = eyeDistance * sinDeg(eyeLatitude);
  double zEye = eyeDistance * cosDeg(eyeLatitude) * cosDeg(eyeLongitude);
  gluLookAt(xEye, yEye, zEye, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  // TODO: Delete this other metod if resubmitting
  // gluLookAt(0.0, 0.0, eyeDistance, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  // glRotated(eyeLatitude, 1.0, 0.0, 0.0);
  // glRotated(-eyeLongitude, 0.0, 1.0, 0.0);

  // Set world positions of the two lights.
  glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
  glLightfv(GL_LIGHT1, GL_POSITION, light1Position);

  // Draw axes.
  if (drawAxes) DrawAxes(2 * PLANET_RADIUS);

  // Draw planet.
  glColor3fv(planetColor);
  glutSolidSphere(PLANET_RADIUS, 72, 36);

  // Draw the cars.
  DrawAllCars();

  glutSwapBuffers();
}

/////////////////////////////////////////////////////////////////////////////
// Update each car's angular position on the great circle by
// the angle increment.
/////////////////////////////////////////////////////////////////////////////
void UpdateCars(void) {
  for (int i = 0; i < NUM_CARS; i++) {
    car[i].angularPos += car[i].angleIncr;
    if (car[i].angularPos > 360.0) car[i].angularPos -= 360.0;
  }
  glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////////////////
// Initializes each car with a random body color, a random rotation
// increment (speed), a random anglular position, and a random great circle.
/////////////////////////////////////////////////////////////////////////////
void InitCars(void) {
  for (int i = 0; i < NUM_CARS; i++) {
    car[i].bodyColor[0] = (float) rand() / RAND_MAX;  // 0.0 to 1.0.
    car[i].bodyColor[1] = (float) rand() / RAND_MAX;  // 0.0 to 1.0.
    car[i].bodyColor[2] = (float) rand() / RAND_MAX;  // 0.0 to 1.0.

    car[i].angleIncr = (double) rand() / RAND_MAX *
                           (CAR_MAX_ANGLE_INCR - CAR_MIN_ANGLE_INCR) +
                       CAR_MIN_ANGLE_INCR;
    // CAR_MIN_ANGLE_INCR to CAR_MAX_ANGLE_INCR.

    car[i].angularPos = (double) rand() / RAND_MAX * 360.0;  // 0.0 to 360.0.

    // The following 3 items defines a random great circle.
    car[i].xzAxis[0] = (double) rand() / RAND_MAX * 2.0 - 1.0;  // -1.0 to 1.0.
    car[i].xzAxis[1] = (double) rand() / RAND_MAX * 2.0 - 1.0;  // -1.0 to 1.0.
    car[i].rotAngle = (double) rand() / RAND_MAX * 360.0;       // 0.0 to 360.0.
  }
}

/////////////////////////////////////////////////////////////////////////////
// The timer callback function.
/////////////////////////////////////////////////////////////////////////////
void MyTimer(int v) {
  UNUSED(v);
  if (showAnimation) {
    UpdateCars();
    glutTimerFunc(MS_BETWEEN_FRAMES, MyTimer, UNUNSED_VAR);
  }
}

/////////////////////////////////////////////////////////////////////////////
// The keyboard callback function.
/////////////////////////////////////////////////////////////////////////////
void MyKeyboard(unsigned char key, int x, int y) {
  UNUSED(x);
  UNUSED(y);
  switch (key) {
  // Quit program.
  case 'q':
  case 'Q':
    exit(0);
    break;

  // Toggle between wireframe and filled polygons.
  case 'w':
  case 'W':
    drawWireframe = !drawWireframe;
    if (drawWireframe)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutPostRedisplay();
    break;

  // Toggle axes.
  case 'x':
  case 'X':
    drawAxes = !drawAxes;
    glutPostRedisplay();
    break;

  // Pause or resume animation.
  case 'p':
  case 'P':
    showAnimation = !showAnimation;
    if (showAnimation) glutTimerFunc(0, MyTimer, UNUNSED_VAR);
    break;

  // Reset to initial view.
  case 'r':
  case 'R':
    eyeLatitude = 0.0;
    eyeLongitude = 0.0;
    eyeDistance = EYE_INIT_DIST;
    glutPostRedisplay();
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////
// The special key callback function.
/////////////////////////////////////////////////////////////////////////////
void MySpecialKey(int key, int x, int y) {
  UNUSED(x);
  UNUSED(y);
  switch (key) {
  case GLUT_KEY_LEFT:
    eyeLongitude -= EYE_LONGITUDE_INCR;
    if (eyeLongitude < -360.0) eyeLongitude += 360.0;
    glutPostRedisplay();
    break;

  case GLUT_KEY_RIGHT:
    eyeLongitude += EYE_LONGITUDE_INCR;
    if (eyeLongitude > 360.0) eyeLongitude -= 360.0;
    glutPostRedisplay();
    break;

  case GLUT_KEY_DOWN:
    eyeLatitude -= EYE_LATITUDE_INCR;
    if (eyeLatitude < EYE_MIN_LATITUDE) eyeLatitude = EYE_MIN_LATITUDE;
    glutPostRedisplay();
    break;

  case GLUT_KEY_UP:
    eyeLatitude += EYE_LATITUDE_INCR;
    if (eyeLatitude > EYE_MAX_LATITUDE) eyeLatitude = EYE_MAX_LATITUDE;
    glutPostRedisplay();
    break;

  case GLUT_KEY_PAGE_UP:
    eyeDistance -= EYE_DIST_INCR;
    if (eyeDistance < EYE_MIN_DIST) eyeDistance = EYE_MIN_DIST;
    glutPostRedisplay();
    break;

  case GLUT_KEY_PAGE_DOWN:
    eyeDistance += EYE_DIST_INCR;
    glutPostRedisplay();
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////
// The reshape callback function.
/////////////////////////////////////////////////////////////////////////////
void MyReshape(int w, int h) {
  winWidth = w;
  winHeight = h;
  glViewport(0, 0, w, h);
}

/////////////////////////////////////////////////////////////////////////////
// The init function. It initializes some OpenGL states.
/////////////////////////////////////////////////////////////////////////////
void MyInit(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0);  // Set black background color.
  glEnable(GL_DEPTH_TEST);           // Use depth-buffer for hidden surface removal.
  glShadeModel(GL_SMOOTH);

  // Set Light 0.
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0Ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
  glEnable(GL_LIGHT0);

  // Set Light 1.
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
  glEnable(GL_LIGHT1);

  glEnable(GL_LIGHTING);

  // Set some global light properties.
  GLfloat globalAmbient[] = {0.1, 0.1, 0.1, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  // Set the universal material properties.
  // The diffuse and ambient components can be changed using glColor*().
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
  glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glEnable(GL_NORMALIZE);  // Let OpenGL automatically renomarlize all normal vectors.
}

/////////////////////////////////////////////////////////////////////////////
// The main function.
/////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  srand(27);  // set random seed

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(winWidth, winHeight);
  glutCreateWindow("main");

  MyInit();
  InitCars();

  // Register the callback functions.
  glutDisplayFunc(MyDisplay);
  glutReshapeFunc(MyReshape);
  glutKeyboardFunc(MyKeyboard);
  glutSpecialFunc(MySpecialKey);
  glutTimerFunc(0, MyTimer, UNUNSED_VAR);

  // Display user instructions in console window.
  printf("Press LEFT ARROW to move eye left.\n");
  printf("Press RIGHT ARROW to move eye right.\n");
  printf("Press DOWN ARROW to move eye down.\n");
  printf("Press UP ARROW to move eye up.\n");
  printf("Press PAGE UP to move closer.\n");
  printf("Press PAGE DN to move further.\n");
  printf("Press 'P' to toggle car animation.\n");
  printf("Press 'W' to toggle wireframe.\n");
  printf("Press 'X' to toggle axes.\n");
  printf("Press 'R' to reset to initial view.\n");
  printf("Press 'Q' to quit.\n\n");

  // Enter GLUT event loop.
  glutMainLoop();
  return 0;
}
