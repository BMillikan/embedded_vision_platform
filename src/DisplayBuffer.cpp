#include <string.h>

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

#include <DisplayBuffer.h>
#include <Utilities.h>

using namespace std;
using namespace Utilities;

DisplayBuffer::DisplayBuffer(int w, int h, const string title) : _fpsLimit(300), _fpsCount(0), _frameCount(0), _pbo(0), _displayTex(0), _bufferTex(0), _cuda_pbo_resource(NULL)
{
    width = w;
    height = h;

    _blockSize = new dim3(16,16);
    _gridSize = new dim3(width / _blockSize->x, height / _blockSize->y);

    _title = title;
	
#ifdef _DEBUG
	cout << "DisplayBuffer::DisplayBuffer() complete\n" << flush;
#endif
}

DisplayBuffer::~DisplayBuffer(void)
{

    delete _blockSize;
    delete _gridSize;
}

//*************************************************
// void InitGLBuffers()
//*************************************************
void DisplayBuffer::InitGLBuffers(void)
{
    if (_pbo)
    {
#ifdef _DEBUG
	cout << "Deleting old CUDA buffer.\n" << flush;
#endif
        // delete old buffer
        checkCudaErrors(cudaGraphicsUnregisterResource(_cuda_pbo_resource));
        glDeleteBuffers(1, &_pbo);
    }

    // create pixel buffer object for display
#ifdef _DEBUG
    cout << "Ready to call glGenBuffers; width " << width << " height " << height << endl << flush;
#endif
    glGenBuffers(1, &_pbo);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, _pbo);
    glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, width*height*sizeof(uchar4), 0, GL_STREAM_DRAW_ARB);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

#ifdef _DEBUG
    cout << " &cuda_pbo_resource " << std::hex << &_cuda_pbo_resource << " pbo " << std::hex  << &_pbo << " cudaGraphicsMapFlagsWriteDiscard " << std::dec << cudaGraphicsMapFlagsWriteDiscard << endl << flush;
#endif
    checkCudaErrors(cudaGraphicsGLRegisterBuffer(&_cuda_pbo_resource, _pbo,cudaGraphicsMapFlagsWriteDiscard));

    // create texture for display
    if (_displayTex)
    {
#ifdef _DEBUG
	cout << "Deleting old CUDA displayTex buffer.\n" << flush;
#endif
        glDeleteTextures(1, &_displayTex);
    }

#ifdef _DEBUG
    cout << "Ready to call glGenTextures; width " << width << " height " << height << endl << flush;
#endif
    glGenTextures(1, &_displayTex);
    glBindTexture(GL_TEXTURE_TYPE, _displayTex);
    glTexImage2D(GL_TEXTURE_TYPE, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_TYPE, 0);

    if( _gridSize )
       delete _gridSize;

    // calculate new grid size
    _gridSize = new dim3(iDivUp(width, _blockSize->x), iDivUp(height, _blockSize->y));

#ifdef _DEBUG
    cout << "InitGlBuffers complete..." << endl << flush;
#endif
}
