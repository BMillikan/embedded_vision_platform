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

// NPP function
#include <npp.h>

// Helper functions
#include <helper_functions.h> // CUDA SDK Helper functions
#include <helper_cuda.h>      // CUDA device initialization helper functions

#include <DisplayBuffer.h>
#include <Utilities.h>

#define REFRESH_DELAY 50 // (ms) 20 Hz

using namespace std;
using namespace Utilities;

map<GLuint, DisplayBuffer *> DisplayHandler::_dict;
int DisplayHandler::_instanceCounter = 0;

DisplayHandler::DisplayHandler(int argc, char *argv[])
{

   DisplayHandler::_instanceCounter++;

   if (DisplayHandler::_instanceCounter > 1)
   {
      cerr << "ERROR: Cannot instantiate more that one DisplayHandler class\n";
      throw DISLPAY_HANDLER_INSTANTIATION_ERROR;
   }

   // use command-line specified CUDA device, otherwise use device with highest Gflops/s
   int devID = findCudaDevice(argc, (const char **)argv);

   // get number of SMs on this GPU
   cudaDeviceProp deviceProps;
   checkCudaErrors(cudaGetDeviceProperties(&deviceProps, devID));

#ifdef _DEBUG
   cout << "CUDA device [" << deviceProps.name << "] has " << deviceProps.multiProcessorCount << " Multi - Processors\n" << flush;
#endif

   checkCudaErrors(cudaSetDevice(devID));

   if (PrintNPPInfo(argc, argv) == false)
   {
      exit(EXIT_SUCCESS);
	     
   }
   Init(&argc, argv);
}

DisplayHandler::~DisplayHandler(void)
{
   for( auto it = _dict.begin(); it != _dict.end(); it++ )
   {
       glutDestroyWindow(it->first);
   }
   glutLeaveMainLoop();
}

bool DisplayHandler::PrintNPPInfo(int argc, char *argv[])
{
   const NppLibraryVersion *libVer = nppGetLibVersion();

#ifdef _DEBUG
   cout << "NPP Library Version " << libVer->major << "." << libVer->minor << "." << libVer->build << endl << flush;
#endif

   int driverVersion, runtimeVersion;
   cudaDriverGetVersion(&driverVersion);
   cudaRuntimeGetVersion(&runtimeVersion);

#ifdef _DEBUG
   cout << "  CUDA Driver  Version: " << (driverVersion/1000) << "." << ((driverVersion%100)/10) << endl << flush;
   cout << "  CUDA Runtime Version: " << (runtimeVersion/1000) << "." << ((runtimeVersion%100)/10) << endl << flush;
#endif

   // Min spec is SM 1.1 devices
   bool bVal = checkCudaCapabilities(1, 1);

   return bVal;
}

void DisplayHandler::Init(int *argc, char **argv)
{
#ifdef _DEBUG
   cout << "Init... \n" << flush;
#endif

   // initialize GLUT callback functions
#ifdef _DEBUG
   cout << "Calling glutInit....\n" << flush;
#endif
   glutInit(argc, argv);
#ifdef _DEBUG
   cout << "Calling glutInitDisplayMode...\n" << flush;
#endif
   glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

#ifdef _DEBUG
   cout <<"End of Init...\n" << flush;
#endif
}

void DisplayHandler::AddDisplayBuffer(DisplayBuffer *tmp)
{
    //DisplayBuffer *tmp = new DisplayBuffer(width, height, 30);
#ifdef _DEBUG
    cout << "DisplayHandler::AddDisplayBuffer().\n" << flush;
    cout << "tmp = " << std::hex << tmp << "\n" << flush;
    cout << "tmp->GetWindowID() = " << tmp->GetWindowID() << "\n" << flush;
#endif

    map<GLuint, DisplayBuffer *>::iterator it = _dict.end();

    DisplayHandler::_dict.insert(it, pair<GLuint, DisplayBuffer *>(tmp->GetWindowID(), tmp));

   if (!isGLVersionSupported(2,0))
   {
       fprintf(stderr, "The following required OpenGL extensions missing:\n\tGL_VERSION_2_0\n\tGL_VERSION_1_5\n");
       exit(EXIT_SUCCESS);
   }

   if( !areGLExtensionsSupported("GL_ARB_pixel_buffer_object") )
   {
      cerr << "The following required OpenGL extensions are missing:\n\tGL_ARB_pixel_buffer_object\n.";
      exit(EXIT_FAILURE);
   }

#ifdef _USE_BUFFER_TEX
   if (!areGLExtensionsSupported("GL_EXT_texture_buffer_object"))
   {
      cerr << "OpenGL extension: GL_EXT_texture_buffer_object missing\n";
      exit(EXIT_FAILURE);
   }

   if (!areGLExtensionsSupported("GL_NV_gpu_program4"))
   {
      cerr << "OpenGL extension: GL_NV_gpu_program4 missing.\n";
      exit(EXIT_FAILURE);
   }
#endif

   glEnable(GL_DEPTH_TEST);

   glutDisplayFunc(DisplayHandler::Display);
   glutKeyboardFunc(DisplayHandler::Keyboard);
   glutMouseFunc(DisplayHandler::Mouse);
   glutMouseWheelFunc(DisplayHandler::MouseWheel);
   glutMotionFunc(DisplayHandler::Motion);
   glutReshapeFunc(DisplayHandler::Reshape);
   glutTimerFunc(REFRESH_DELAY, DisplayHandler::TimerEvent, 0);
   glutIdleFunc(DisplayHandler::Idle);

#if defined (__APPLE__) || defined(MACOSX)
   atexit(DisplayHandler::Cleanup);
#else
   glutCloseFunc(DisplayHandler::Cleanup);
#endif

#ifdef _DEBUG
    cout << "DisplayHandler::AddDisplayBuffer() complete....\n" << flush;
#endif
}

void DisplayHandler::TimerEvent(int value)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::TimerEvent for Window " << (int)winID << "\n" << flush;
#endif

   if( winID )
   {
      glutPostRedisplay();
      //DisplayBuffer *p = DisplayHandler::_dict[winID];

      glutTimerFunc(REFRESH_DELAY, DisplayHandler::TimerEvent, 0);
   }
}
//*************************************************
// void MouseWheel(int button, int dir, int x, int y)
//*************************************************
void DisplayHandler::MouseWheel(int button, int dir, int x, int y)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::MouseWheel: button " << button << " dir " << dir << " x " << x << " y " << y << " for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

   p->MouseWheel(button, dir, x, y);

   glutReportErrors();

}

//*************************************************
// void MouseWheel(int button, int dir, int x, int y)
//*************************************************
void DisplayHandler::Mouse(int button, int state, int x, int y)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::Mouse: button " << button << " state " << state << " x " << x << " y " << y << " for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

   p->Mouse(button, state, x, y);

}


//*************************************************
// void Cleanup()
//*************************************************
void DisplayHandler::Cleanup(void)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::Cleanup for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

   p->Cleanup();


}

//*************************************************
// void Keyboard(unsigned char key, int x, int y)
//*************************************************
void DisplayHandler::Keyboard(unsigned char key, int x, int y)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::Keyboard: key " << key << " x " << x << " y " << y << " for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

   p->Keyboard(key, x, y);

   glutPostRedisplay();

}

//*************************************************
// void Motion(int x, int y)
//*************************************************
void DisplayHandler::Motion(int x, int y)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::Motion: x " << x << " y " << y << " for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

   p->Motion(x, y);

   glutPostRedisplay();

}

//*************************************************
// void Reshape(int x, int y)
//*************************************************
void DisplayHandler::Reshape(int x, int y)
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHandler::Reshape: x " << x << " y " << y << " for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

   p->Reshape(x, y);

   glutPostRedisplay();
 
}

//*************************************************
// void DisplayHandler::Display()
//*************************************************
void DisplayHandler::Display()
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHander::Display() for Window " << (int)winID << "\n" << flush;
#endif

   DisplayBuffer *p = DisplayHandler::_dict[winID];

#ifdef _DEBUG
   cout << "DisplayHandler::Display() for class " << std::hex << p << std::dec << endl << flush;
#endif

   p->Display();

   glutPostRedisplay();

}

//*************************************************
// void DisplayHandler::Idle()
//*************************************************
void DisplayHandler::Idle()
{
   GLuint winID = glutGetWindow();

#ifdef _DEBUG
   cout << "DisplayHander::Idle() for Window " << (int)winID << "\n" << flush;
#endif

   glutPostRedisplay();
}

//*************************************************
// void DisplayHandler::Run()
//*************************************************
void DisplayHandler::Run()
{
   glutMainLoop();
}
