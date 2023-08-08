#include <signal.h>

// OpenGL Graphics includes
#define HELPERGL_EXTERN_GL_FUNC_IMPLEMENTATION
#include <helper_gl.h>
#if defined(__APPLE__) || defined(MACOSX)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// CUDA system and GL includes
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

// Helper functions
#include <helper_functions.h> // CUDA SDK Helper functions
#include <helper_cuda.h>      // CUDA device initialization helper functions

#include <DisplayHandler.h>
#include <DisplayBuffer_Earth3D.h>
#include <Utilities.h>

using namespace Utilities;
using namespace std;

DisplayHandler *dh;

void SignalHandler(int sig)
{
   signal(sig, SIG_IGN);

   delete dh;
}

int main(int argc, char *argv[])
{
   printf("Initializing display handler...\n");
   dh = new DisplayHandler(argc, argv);

   printf("Initializing displlay buffer for earth 3D\n");
   DisplayBuffer_Earth3D db(600, 600, "Earth 3D OpenGL");

   printf("Setting log level...\n");
   logger.SetLogLevel(WARNING);

   /* Setup handler for Ctrl-C to terminate program */
   printf("Setup signal handler\n");
   signal(SIGINT, SignalHandler);


   /* Run OpenGL */
   printf("Running OpenGL...\n");
   dh->Run();

   /* Exit */
   printf("Exiting...\n");

   return EXIT_SUCCESS;
}
