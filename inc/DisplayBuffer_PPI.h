#ifndef __DISPLAY_BUFFER_PPI_H__
#define __DISPLAY_BUFFER_PPI_H__

#include <vector>
#include <DisplayBuffer.h>
#ifndef _WIN32
#include <GPSINS.h>
#endif

using namespace std;

class DisplayBuffer_PPI : public DisplayBuffer
{
public:
   DisplayBuffer_PPI(int width, int height, string title);
   ~DisplayBuffer_PPI(void);

   void UpdateGPSPos_ECEF(vector<float> pos);
   void UpdateGPSPos_LLA(vector<float> pos);
#ifndef _WIN32
   void UpdateGPSINS(GPSINS_MSG &msg);
#endif

   // Event Handlers
   virtual void MouseWheel(int button, int dir, int x, int y){}
   virtual void Display(void);
   void Reshape(int x, int y){}
   void Keyboard(unsigned char key, int /*x*/, int /*y*/){}
   void Motion(int x, int y){}
   void Mouse(int button, int state, int x, int y){}
   void Cleanup(void);

private:
#ifdef _WIN32
	HANDLE _lock;
#else
   pthread_mutex_t _lock;
#endif

   int _id;
   StopWatchInterface *_timer;

   int _offsetx, _offsety;

   // Internal methods
   void computeFPS();
   void plotCircle(float radius);
   void drawText(float x, float y, const char *thestring, void *font);
   void plotLine(float x1,float y1, float x2, float y2);

   vector<float> * _curGPSCoord;
};

#endif // __DISPLAY_BUFFER_H__
