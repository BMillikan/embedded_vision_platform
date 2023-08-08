#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>
#include <time.h>
#include <string>
#include <fstream> /* ofstream */

#ifdef _WIN32
	#ifdef ERROR
	#undef ERROR
	#endif
#endif

using namespace std;

typedef enum _log_level {
	DEBUG,
	INFO,
	WARNING,
	ERROR,
        NUM_LOG_LEVELS } LOG_LEVEL;

class Logger
{
public:
   Logger();
   Logger(const string &filename);
   ~Logger();

   inline void SetFilename(const string &filename){ _filename = filename; }

   void Write(LOG_LEVEL level, const string &message);
   void Write(LOG_LEVEL level, const stringstream &ss);
   void Write(LOG_LEVEL level, const ostream &os);

   inline void SetLogLevel(LOG_LEVEL level) { _outputLevel = level; }
private:
   ofstream _outfile;
   timespec create_time;
   timespec curr_time;
   double stime;
   double ctime;
   double prev_time;

   const char *GetTypeStr(LOG_LEVEL level);

   LOG_LEVEL _outputLevel;

   string _filename;

   bool _echoToScreen;
};

#endif // __LOGGER_H__
