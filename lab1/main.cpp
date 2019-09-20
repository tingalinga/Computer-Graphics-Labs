// ============================================================
// STUDENT NAME: Soeren Hougaard Mulvad
// STUDENT NO.: A0207552J
// NUS EMAIL ADDRESS: e0445646@u.nus.edu
// COMMENTS TO GRADER: This program has been developed and tested on macOS
// and compiled with: "gcc main.cpp -o main -framework GLUT -framework OpenGL
// -Wall -Werror -Wextra -pedantic -Wno-deprecated"
// There may therefore be problems with library functions, although
// I believe there shouldn't.
// The library <ctime> has been added as to allow seeding of our random
// function with a new value each time the program is run.
// ============================================================

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433832795

#define MAX_NUM_OF_DISCS 200  // Limit the number of discs.
#define MIN_RADIUS 10.0       // Minimum radius of disc.
#define MAX_RADIUS 50.0       // Maximum radius of disc.
#define NUM_OF_SIDES 18       // Number of polygon sides to approximate a disc.
#define RADIANS_VERTEX \
  2.0 * PI / NUM_OF_SIDES;  // Angle in radians between vertices

#define MIN_X_SPEED 1.0   // Minimum speed of disc in X direction.
#define MAX_X_SPEED 20.0  // Maximum speed of disc in X direction.
#define MIN_Y_SPEED 1.0   // Minimum speed of disc in Y direction.
#define MAX_Y_SPEED 20.0  // Maximum speed of disc in Y direction.

#define DESIRED_FPS 30  // Approximate desired number of frames per second.
#define MS_BETWEEN_FRAMES (unsigned int) 1000 / DESIRED_FPS

// A simple expression for silencing compiler warnings for
// unused parameters without altering program flow
#define UNUSED(expr) \
  do {               \
    (void) (expr);   \
  } while (0)
#define UNUNSED_VAR 0

/////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////

typedef struct discType {
  double pos[2];           // The X and Y coordinates of the center of the disc.
  double speed[2];         // The velocity of the disc in X and Y directions. Can be
                           // negative.
  double radius;           // Radius of the disc.
  unsigned char color[3];  // R, G, B colors of the disc.
} discType;

int numDiscs = 0;                 // Number of discs that have been added.
discType disc[MAX_NUM_OF_DISCS];  // Array for storing discs.

bool drawWireframe = false;  // Draw polygons in wireframe if true,
                             // otherwise polygons are filled.

int winWidth = 800;   // Window width in pixels.
int winHeight = 600;  // Window height in pixels.

// Returns a random double in range min..max
double getRand(int min, int max) {
  double randomReal = (double) rand() / RAND_MAX;  // 0.0 to 1.0
  return (double) (max - min) * randomReal + (double) min;
}

// Returns a random double in range -max..-min or min..max
double getRandVelocity(int min, int max) {
  double velocity = getRand(min, max);

  // Make velocity negative half of the time
  bool shouldUsePositive = rand() % 2 == 1;
  return shouldUsePositive ? velocity : -velocity;
}

// Returns an unsigned char in range 0..255
unsigned char getRandRgb() {
  return (unsigned char) rand() % 256;
}

/////////////////////////////////////////////////////////////////////////////
// Draw the disc in its color using GL_TRIANGLE_FAN.
/////////////////////////////////////////////////////////////////////////////
void DrawDisc(const discType* d) {
  static bool firstTime = true;
  static double unitDiscVertex[NUM_OF_SIDES + 1][2];

  // Pre-compute and store the vertices' positions of a unit-radius disc.
  // This is done using simple trigonometry in a counterclockwise fashion.
  if (firstTime) {
    for (int i = 0; i < NUM_OF_SIDES + 1; ++i) {
      double currentRadian = (double) i * RADIANS_VERTEX;
      unitDiscVertex[i][0] = cos(currentRadian);
      unitDiscVertex[i][1] = sin(currentRadian);
    }
    firstTime = false;
  }

  double centerX = d->pos[0];
  double centerY = d->pos[1];
  double r = d->radius;

  // Set color for disc d
  glColor3ubv(d->color);

  // clang-format off
  glBegin(GL_TRIANGLE_FAN);
    // First draw the center of the triangle
    glVertex2f(centerX, centerY);

    // Afterwards draw all the other vertices using the unitDiscVertex
    for (int i = 0; i < NUM_OF_SIDES + 1; ++i)
      glVertex2f(centerX + r * unitDiscVertex[i][0],
                 centerY + r * unitDiscVertex[i][1]);
  glEnd();
  // clang-format on
}

/////////////////////////////////////////////////////////////////////////////
// The display callback function.
/////////////////////////////////////////////////////////////////////////////
void MyDisplay(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  if (drawWireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for (int i = 0; i < numDiscs; i++)
    DrawDisc(&disc[i]);

  glutSwapBuffers();
}

/////////////////////////////////////////////////////////////////////////////
// The mouse callback function.
// If mouse left button is pressed, a new disc is created with its center
// at the mouse cursor position. The new disc is assigned the followings:
// (1) a random radius between MIN_RADIUS and MAX_RADIUS,
// (2) a random speed in X direction in the range
//     from -MAX_X_SPEED to -MIN_X_SPEED, and from MIN_X_SPEED to MAX_X_SPEED.
// (3) a random speed in Y direction in the range
//     from -MAX_Y_SPEED to -MIN_Y_SPEED, and from MIN_Y_SPEED to MAX_Y_SPEED.
// (4) R, G, B color, each ranges from 0 to 255.
/////////////////////////////////////////////////////////////////////////////
void MyMouse(int btn, int state, int x, int y) {
  if (btn != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
    return;

  if (numDiscs >= MAX_NUM_OF_DISCS) {
    printf("Already reached maximum number of discs.\n");
    return;
  }

  // Translate y to flipped coordinate system
  y = winHeight - 1 - y;

  double r = getRand((int) MIN_RADIUS, (int) MAX_RADIUS);
  double xv = getRandVelocity((int) MIN_X_SPEED, (int) MAX_X_SPEED);
  double yv = getRandVelocity((int) MIN_Y_SPEED, (int) MAX_Y_SPEED);

  discType newDisc = {{(double) x, (double) y},
                      {xv, yv},
                      r,
                      {getRandRgb(), getRandRgb(), getRandRgb()}};

  disc[numDiscs++] = newDisc;

  glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////////////////
// The reshape callback function.
// It also sets up the viewing.
/////////////////////////////////////////////////////////////////////////////
void MyReshape(int w, int h) {
  winWidth = w;
  winHeight = h;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluOrtho2D(0, w, 0, h);

  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////////////////
// The keyboard callback function.
/////////////////////////////////////////////////////////////////////////////
void MyKeyboard(unsigned char key, int unusedX, int unusedY) {
  // Because of the type signature of MyKeyboard, we have to accept
  // two extra ints. We do however not have any use for it. Therefore
  // we silence the compiler warning
  UNUSED(unusedX);
  UNUSED(unusedY);

  switch (key) {
  // Quit program.
  case 'q':
  case 'Q':
    exit(0);
    break;

  // Toggle wireframe or filled polygons.
  case 'w':
  case 'W':
    drawWireframe = !drawWireframe;
    glutPostRedisplay();
    break;

  // Reset and erase all discs.
  case 'r':
  case 'R':
    numDiscs = 0;
    glutPostRedisplay();
    break;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Updates the positions of all the discs.
//
// Increments the position of each disc by its speed in each of the
// X and Y directions. Note that the speeds can be negative.
// At its new position, if the disc is entirely or partially outside the
// left window boundary, then shift it right so that it is inside the
// window and just touches the left window boundary. Its speed in the X
// direction must now be reversed (negated). Similar approach is
// applied for the cases of the right, top, and bottom window boundaries.
/////////////////////////////////////////////////////////////////////////////
void UpdateAllDiscPos(void) {
  for (int i = 0; i < numDiscs; i++) {
    discType& curDisc = disc[i];
    // Update position
    curDisc.pos[0] += curDisc.speed[0];
    curDisc.pos[1] += curDisc.speed[1];

    double centerX = curDisc.pos[0];
    double centerY = curDisc.pos[1];
    double r = curDisc.radius;

    // Respectively handle if outside left border and if outside right border
    if (centerX - r <= 0) {
      curDisc.pos[0] = r;
      curDisc.speed[0] = -curDisc.speed[0];
    } else if (centerX + r >= winWidth) {
      curDisc.pos[0] = winWidth - r;
      curDisc.speed[0] = -curDisc.speed[0];
    }

    // Respectively handle if outside bottom border and if outside top border
    if (centerY - r <= 0) {
      curDisc.pos[1] = r;
      curDisc.speed[1] = -curDisc.speed[1];
    } else if (centerY + r >= winHeight) {
      curDisc.pos[1] = winHeight - r;
      curDisc.speed[1] = -curDisc.speed[1];
    }
  }
  glutPostRedisplay();
}

/////////////////////////////////////////////////////////////////////////////
// The timer callback function.
// It calls the function that animates the disc, and then sets up a callback
// for itself after the desired amount of milliseconds to reach the
// approximate FPS have passed
/////////////////////////////////////////////////////////////////////////////
void MyTimer(int unusedInt) {
  // Because of the type signature of glutTimerFunc, we have to accept
  // an int. Silence compiler warning
  UNUSED(unusedInt);

  // Animate
  UpdateAllDiscPos();

  // Call MyTimer again after the desired milliseconds have passed.
  glutTimerFunc(MS_BETWEEN_FRAMES, MyTimer, UNUNSED_VAR);
}

/////////////////////////////////////////////////////////////////////////////
// The init function.
// It initializes some OpenGL states.
/////////////////////////////////////////////////////////////////////////////
void MyInit(void) {
  glClearColor(0.0, 0.0, 0.0, 1.0);  // Black background color.
  glShadeModel(GL_FLAT);
}

/////////////////////////////////////////////////////////////////////////////
// The main function.
/////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  srand(27);  // set random seed

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(winWidth, winHeight);
  glutCreateWindow("main");

  MyInit();

  // Register the callback functions.
  glutDisplayFunc(MyDisplay);
  glutReshapeFunc(MyReshape);
  glutMouseFunc(MyMouse);
  glutKeyboardFunc(MyKeyboard);
  glutTimerFunc(0, MyTimer, UNUNSED_VAR);

  // Display user instructions in console window.
  printf("Click LEFT MOUSE BUTTON in window to add new disc.\n");
  printf("Press 'w' to toggle wireframe.\n");
  printf("Press 'r' to erase all discs.\n");
  printf("Press 'q' to quit.\n\n");

  // Enter GLUT event loop.
  glutMainLoop();
  return 0;
}
