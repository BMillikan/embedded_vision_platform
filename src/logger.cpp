#include <iostream>  /* std::cout */
#include <ctime>     /* strftime */
#include <sstream>   /* stringstream */
#include <iomanip>   /* std::setw, std::setprecision */

#ifdef _WIN32
#include <windows.h>
#define CLOCK_THREAD_CPUTIME_ID 0

int clock_gettime(int, struct timespec *spec)      //C-file part
{
	__int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
	wintime -= 116444736000000000i64;  //1jan1601 to 1jan1970
	spec->tv_sec = wintime / 10000000i64;           //seconds
	spec->tv_nsec = wintime % 10000000i64 * 100;      //nano-seconds
	return 0;
}
#endif

#include <logger.h>
#include <Utilities.h>

using namespace Utilities;
using namespace std;

const char *logColor[NUM_LOG_LEVELS] = { CYN, GRN, YEL, RED };

Logger::Logger() : _outputLevel(WARNING), _echoToScreen(false)
{
#ifdef _DEBUG
   _outputLevel = DEBUG;
   _echoToScreen = true;
#endif
   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &create_time);

   stime = create_time.tv_sec + create_time.tv_nsec*1e-9;
   prev_time = stime;

   /* Use default log filename */
   time_t rawtime;
   struct tm * timeinfo;
   char buffer[80];

   time(&rawtime);
   timeinfo = localtime(&rawtime);

   strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H_%M_%S", timeinfo);
   string str(buffer);

   stringstream ss;

   ss << "Log_" << str << ".log";
   _filename = ss.str();

   _outfile.open(_filename, ios::out | ios::trunc );

   if( !_outfile.is_open() )
   {
      ss.str(std::string());
      ss << "Error: Cannot open " << _filename;
      logger.Write(ERROR,ss);
   }

   Write(INFO, "Start of Log");
}

Logger::Logger(const string &filename) : _filename(filename)
{
   stringstream ss;
   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &create_time);

   stime = create_time.tv_sec + create_time.tv_nsec*1e-9;
   prev_time = stime;

   _outfile.open(_filename, ios::out | ios::trunc);

   if( !_outfile.is_open() )
   {
      cerr << RED << "Error: Cannot open " << _filename << WHT << endl;
      return;
   }

   Write(INFO, "Start of Log");
}

Logger::~Logger()
{
   if( _outfile.is_open() )
   {
      Write(INFO, "End of Log");

      _outfile.close();
   }
}

const char *Logger::GetTypeStr(LOG_LEVEL level)
{
   switch(level)
   {
   case DEBUG:
      return "DEBUG";
      break;
   case INFO:
      return "INFO";
      break;
   case WARNING:
      return "WARNING";
      break;
   case ERROR:
      return "ERROR";
      break;
   default:
      return "UNDEFINED";
      break;
   }
}

void Logger::Write(LOG_LEVEL level, const stringstream &ss)
{
   Write(level, ss.str());
}

void Logger::Write(LOG_LEVEL level, const string &message)
{
   stringstream ss;

   clock_gettime(CLOCK_THREAD_CPUTIME_ID, &curr_time);

   ctime = curr_time.tv_sec + curr_time.tv_nsec*1e-9;

   if( level >= _outputLevel )
   {
      ss << fixed << setw(10) << setprecision(5) << (ctime - stime) << ", " << ctime << ", " << (ctime - prev_time) << ", " << GetTypeStr(level) << " : " << message;
      _outfile << ss.str() << endl;
      if(_echoToScreen)
      {
         cout << logColor[level] << ss.str() << WHT << endl;
      }
   }

   prev_time = ctime;
}
