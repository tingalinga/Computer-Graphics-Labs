// To disable deprecation warnings for using vsprintf() and _ftime64().
#define _CRT_SECURE_NO_WARNINGS

#include "Util.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MSG_BUF_SIZE 1024

// Outputs an error message to the stderr and exits program.
void Util::ErrorExit(char *format, ...) {
  va_list args;
  static char buffer[MSG_BUF_SIZE];
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  fprintf(stderr, "ERROR: %s\n\n", buffer);
  exit(1);
}

// Outputs an error message to the stderr and exits program.
// Needs source file name and line number.
void Util::ErrorExitLoc(const char *srcfile, int lineNum, char *format, ...) {
  va_list args;
  static char buffer[MSG_BUF_SIZE];
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  fprintf(stderr, "ERROR at \"%s\" (line %d):\n%s\n\n", srcfile, lineNum, buffer);
  exit(1);
}

// Outputs a warning message to the stderr.
void Util::ShowWarning(char *format, ...) {
  va_list args;
  static char buffer[MSG_BUF_SIZE];
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  fprintf(stderr, "WARNING: %s\n\n", buffer);
}

// Outputs a warning message to the stderr.
// Needs source file name and line number.
void Util::ShowWarningLoc(const char *srcfile, int lineNum, char *format, ...) {
  va_list args;
  static char buffer[MSG_BUF_SIZE];
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  fprintf(stderr, "WARNING at \"%s\" (line %d):\n%s\n\n", srcfile, lineNum, buffer);
}

//============================================================================

// Returns time in seconds (plus fraction of a second) since midnight (00:00:00),
// January 1, 1970, coordinated universal time (UTC).
double Util::GetCurrRealTime(void) {
#ifdef _WIN32
  struct _timeb timebuffer;
  _ftime(&timebuffer);
#else
  struct timeb timebuffer;
  ftime(&timebuffer);
#endif
  return ((double) timebuffer.time + ((double) timebuffer.millitm / 1000.0));
}

// Returns cpu time in seconds (plus fraction of a second) since the
// start of the current process.
double Util::GetCurrCPUTime(void) {
  return ((double) clock()) / CLOCKS_PER_SEC;
}
