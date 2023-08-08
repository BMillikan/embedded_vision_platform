#ifndef __DISPLAY_BUFFER_H__
#define __DISPLAY_BUFFER_H__

#include <DisplayHandler.h>
#include <iomanip>
#include <Property.h>

#define REFRESH_DELAY 50 // (ms) 20 Hz
#define GL_TEXTURE_TYPE GL_TEXTURE_RECTANGLE_ARB
//#define GL_TEXTURE_TYPE GL_TEXTURE_2D

#ifdef _WIN32
#undef GetWindowID
#endif

using namespace std;

class DisplayBuffer
{
public:
   DisplayBuffer(int width, int height, string title);
   ~DisplayBuffer(void);

   GLuint GetWindowID(void){ return _glWinID; }

   // *REQUIRED* Event Handlers
   virtual void MouseWheel(int button, int dir, int x, int y) = 0;
   virtual void Display(void) = 0;
   virtual void Reshape(int x, int y) = 0;
   virtual void Keyboard(unsigned char key, int /*x*/, int /*y*/) = 0;
   virtual void Motion(int x, int y) = 0;
   virtual void Mouse(int button, int state, int x, int y) = 0;
   virtual void Cleanup(void) = 0;
   //virtual void Idle(void) = 0;

   //virtual int GetRefreshDelay(void){;

   Property<int> width;
   Property<int> height;

   Property<GLuint> winID;

protected:
   GLuint _pbo;
   GLuint _displayTex;
   GLuint _bufferTex;
   struct cudaGraphicsResource *_cuda_pbo_resource; // handles OpenGL-CUDA exchange
   GLuint _glWinID;

   virtual void computeFPS(void) = 0;
   virtual void InitGLBuffers(void);

   StopWatchInterface *_timer;
   int _frameCount;
   int _fpsCount;
   int _fpsLimit;

   dim3 *_blockSize;
   dim3 *_gridSize;

   string _title;
};

#endif // __DISPLAY_BUFFER_H__
