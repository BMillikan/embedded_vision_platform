#include <string>
#include <iostream>
#include <cmath>

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

#include <DisplayBuffer_PPI.h>
#include <Utilities.h>

using namespace std;
using namespace Utilities;

DisplayBuffer_PPI::DisplayBuffer_PPI(int w, int h, const string title) : DisplayBuffer(w, h, title), _offsetx(200), _offsety(100)
{
    glutInitWindowPosition(_offsetx, _offsety);
    glutInitWindowSize(width, height);
    _glWinID = glutCreateWindow(_title.c_str());

    _curGPSCoord = new vector<float>(3, 0.0f);

#ifdef _WIN32
	if (!(_lock = CreateMutex(NULL, FALSE, NULL)))
	{
		stringstream ss;
		ss << "Error: Mutex init failed\n";
		logger.Write(ERROR, std::string(ss.str()).c_str());

		cerr << RED << ss.str() << WHT << flush;
		return;
	}
#else
    /* Create the mutex */
    if( pthread_mutex_init(&_lock, NULL) != 0 )
    {
       stringstream ss;
       ss << "Error: Mutex init failed\n";
       logger.Write(ERROR, std::string(ss.str()).c_str());

       cerr << RED << ss.str() << WHT << flush;
       return;
    }
#endif

#ifdef _DEBUG
    cout << "Adding display buffer to handler.\n" << flush;
#endif
    DisplayHandler::AddDisplayBuffer(this);

	sdkCreateTimer(&_timer);
}

DisplayBuffer_PPI::~DisplayBuffer_PPI(void)
{
   delete _curGPSCoord;

   sdkDeleteTimer(&_timer);
}

//*************************************************
// void DisplayBuffer_PPI::plotCircle(radius)
//*************************************************
void DisplayBuffer_PPI::plotCircle(float radius)
{
   float x,y;

   glBegin(GL_LINE_STRIP);
      for (int i=0; i<360; i++)
      {
         x = radius*cos((float)i*M_PI/(float)180.);
         y = radius*sin((float)i*M_PI/(float)180.);
         glVertex2f(x, y);
      }
   glEnd();
}


//*************************************************
// void DisplayBuffer_PPI::plotLine(x1,y1,x2,y2)
//*************************************************
void DisplayBuffer_PPI::plotLine(float x1,float y1, float x2, float y2)
{
   glBegin(GL_LINE_STRIP);
      glVertex2f(x1, y1);
      glVertex2f(x2, y2);
   glEnd();
}

//*************************************************
// void DisplayBuffer_PPI::drawText()
//*************************************************
void DisplayBuffer_PPI::drawText(float x, float y, const char *thestring, void *font)
{
   glRasterPos2f(x,y);
   int len=(int)strlen(thestring);
   for(int i=0;i<len;i++)
   {
      glutBitmapCharacter(font, thestring[i]);
   }
}

//*************************************************
// void DisplayBuffer_PPI::Display()
//*************************************************
void DisplayBuffer_PPI::Display(void)
{
   sdkStartTimer(&_timer);

    // display results
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
       glScalef(1.0, 1.0, 1.0);
       drawText(-.8,.8,"Test",GLUT_BITMAP_TIMES_ROMAN_10);
       // PPI Circles
       glTranslatef(0., 0., 0.0);
       glColor3f(.5, 0.5, 1.0);
       for(float radius=0.1;radius<1.1;radius+=.1)
       {
          plotCircle(radius);
       }
       // PPI Lines
       for(float angle=0.;angle<2*M_PI;angle+=M_PI/8.)
       {
          plotLine(0,0,cos(angle),sin(angle));
       }

    glPopMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glutSwapBuffers();
    glutReportErrors();

   sdkStopTimer(&_timer);

   computeFPS();
}

//*************************************************
// void computeFPS()
//*************************************************	
void DisplayBuffer_PPI::computeFPS()
{
   _frameCount++;
   _fpsCount++;

   if (_fpsCount == _fpsLimit)
   {
      float ifps = 1.0f / (sdkGetAverageTimerValue(&_timer) / 1000.f);
      ostringstream ss;
      ss << _title << ": " << std::fixed << std::setprecision(1) << ifps;

      string fps = ss.str();
      glutSetWindowTitle(fps.c_str());
      _fpsCount = 0;

      sdkResetTimer(&_timer);
   }
}

//*************************************************
// void Cleanup()
//*************************************************
void DisplayBuffer_PPI::Cleanup(void)
{
   sdkDeleteTimer(&_timer);
}

//*************************************************
// void UpdateGPSPos_ECEF()
//*************************************************
void DisplayBuffer_PPI::UpdateGPSPos_ECEF(vector<float> pos)
{
    float d = dist(pos);
#ifdef _WIN32
	DWORD res = WaitForSingleObject(_lock, 0);
	if (res == WAIT_OBJECT_0)
	{
		(*_curGPSCoord)[0] = pos[0] / d;
		(*_curGPSCoord)[1] = pos[1] / d;
		(*_curGPSCoord)[2] = pos[2] / d;

		ReleaseMutex(_lock);
	}
#else
    if( !pthread_mutex_trylock(&_lock) )
    { 
       (*_curGPSCoord)[0] = pos[0] / d;
       (*_curGPSCoord)[1] = pos[1] / d;
       (*_curGPSCoord)[2] = pos[2] / d;

       pthread_mutex_unlock(&_lock);
    }
#endif
}
  
//*************************************************
// void UpdateGPSPos_LLA()
//*************************************************
void DisplayBuffer_PPI::UpdateGPSPos_LLA(vector<float> pos)
{
   vector<float> ecef(3, 0);

   GeodeticToECEF(pos, 6378137.0f, &ecef);

   //float d = dist(ecef);
#ifdef _WIN32
   DWORD res = WaitForSingleObject(_lock, 0);
   if (res == WAIT_OBJECT_0)
   {
	   (*_curGPSCoord)[0] = ecef[0];
	   (*_curGPSCoord)[1] = ecef[1];
	   (*_curGPSCoord)[2] = ecef[2];

	   ReleaseMutex(_lock);
   }
#else
   if( !pthread_mutex_trylock(&_lock) )
   {
      (*_curGPSCoord)[0] = ecef[0];
      (*_curGPSCoord)[1] = ecef[1];
      (*_curGPSCoord)[2] = ecef[2];

      pthread_mutex_unlock(&_lock);
   }
#endif
}
#ifndef _WIN32
//*************************************************
// void UpdateGPSPos_LLA()
//*************************************************
void DisplayBuffer_PPI::UpdateGPSINS(GPSINS_MSG &msg)
{
   vector<double> lla(3, 0);
   vector<double> ecef(3, 0);

   lla[0] = DEG2RAD(msg.lla_[0]);
   lla[1] = DEG2RAD(msg.lla_[1]);
   //lla[2] = msg.lla_[2];

#ifdef _DEBUG
   cout << GRN << "Convert LLA lat = " << lla[0] << " lon = " << lla[1] << " alt = " << lla[2] << " to ECEF from GPS/INS msg...\n" << WHT << flush;
#endif
   GeodeticToECEF(lla, 6378137.0, &ecef);

#ifdef _DEBUG
   cout << GRN "Current coord is (" << ecef[0] << "," << ecef[1] << "," << ecef[2] << ")...\n" << WHT << flush;
#endif

#ifdef _DEBUG
   cout << GRN << "Calculate vector Eucidean distance" << WHT << endl << flush;
#endif
   //double d = dist(ecef);

#ifdef _DEBUG
   cout << GRN << "Attempting to update camera pos based on ECEF coord...." << WHT << endl << flush;
#endif
   if( !pthread_mutex_trylock(&_lock) )
   {
      (*_curGPSCoord)[0] = (float)ecef[0];
      (*_curGPSCoord)[1] = (float)ecef[1];
      (*_curGPSCoord)[2] = (float)ecef[2];

      pthread_mutex_unlock(&_lock);
   }
}
#endif