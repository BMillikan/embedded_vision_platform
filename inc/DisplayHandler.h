#ifndef _DISPLAY_HANDLER_H_
#define _DISPLAY_HANDLER_H_

#include <map>

#ifdef _WIN32
#include <windows.h>
#endif

// Forward Declaration
class DisplayBuffer;

using namespace std;

// C++ wrapper for C OpenGL routines
class DisplayHandler
{
public:

   DisplayHandler(int argc, char *argv[]);
   ~DisplayHandler(void);

   static void AddDisplayBuffer(DisplayBuffer *tmp);

   // OpenGL Handler Functions
   static void TimerEvent(int value);
   static void MouseWheel(int button, int dir, int x, int y);
   static void Mouse(int button, int state, int x, int y);
   static void Cleanup(void);
   static void Keyboard(unsigned char key, int x, int y);
   static void Reshape(int x, int y);
   static void Display(void);
   static void Idle(void);
   static void Motion(int x, int y);
   void Run(void);

private:
   static map<GLuint, DisplayBuffer *> _dict;
   static int _instanceCounter;

   void Init(int *argc, char *argv[]);
   bool PrintNPPInfo(int argc, char *argv[]);

};

#endif // _DISPLAY_HANDLER_H_
