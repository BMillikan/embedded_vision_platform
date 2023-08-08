#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#if 0
#include <errno.h>
#include <sys/ioctl.h>
#include <sstream>
#include <vector>
#include <time.h>
#include "logger.h"
#else
#include <vector>   /* std::vector */
#include <time.h>   /* struct timespec */
#include <logger.h> /* logger() */
#include <cstring>  /* memset */
#ifndef _WIN32
#include <sys/ioctl.h> /* ioctl() */ 
#else
#include <windows.h>
#endif
#include <string>
#include <cmath>
#endif

using namespace std;


// Useful macros
#ifndef RAD2DEG
#define RAD2DEG(x) (x * 180.0 / M_PI)
#endif

#ifndef DEG2RAD
#define DEG2RAD(x) (x * M_PI / 180.0)
#endif

#ifndef MAX
#define MAX(a,b) ((a < b) ? b : a)
#endif

namespace Utilities
{
  // Define COLOR CODES
  #define RED   "\x1B[31m"
  #define GRN   "\x1B[32m"
  #define YEL   "\x1B[33m"
  #define BLU   "\x1B[34m"
  #define MAG   "\x1B[35m"
  #define CYN   "\x1B[36m"
  #define WHT   "\x1B[37m"
  #define RESET "\x1B[0m"

  #define CLEAR(x) memset(&(x), 0, sizeof(x))

  void Exit(const string &str);
  void ErrnoExit(const string &s);
  void ErrnoPrint(const string &s);

#ifndef _WIN32
  inline int xioctl(int fh, int request, void *arg)
  {
     return ioctl(fh, request, arg);
  }
#else
  inline int xioctl(int fh, int request, void *arg)
  {
	  return 0;
  }
#endif
  static Logger logger;

  struct timedata {
     timedata(uint64_t u) {
        w_ = u % 1000000000;
        millisec_ = (int)( w_ % 1000 );
        w_ /= 1000;
        sec_ = (int)( w_ % 100 );
        w_ /= 100;
        min_ = (int)( w_ % 100 );
        hour_ = (int)( w_ / 100 );
        v_ = u / 1000000000;
        day_ = (int)( v_ % 100 );
        v_ /= 100;
        month_ = (int)( v_ % 100 );
        year_ = (int)( v_ / 100 );
     }
     unsigned long long w_;
     unsigned millisec_;
     unsigned sec_;
     unsigned min_;
     unsigned hour_;
     unsigned long long v_;
     unsigned day_;
     unsigned month_;
     unsigned year_;
  };

  void msleep(unsigned long millisec);

  bool readDataFromFile(const string& fileName, unsigned char *buffer, int length);
  bool readDataFromFile(const string& fileName, string &data);
  void GeodeticToECEF(double lat, double lon, double alt, double *X, double *Y, double *Z);
  void GeodeticToECEF(float lat, float lon, float alt, float *X, float *Y, float *Z);
  void GeodeticToECEF( vector<double> lla, double radius, vector<double> *ecef );
  void GeodeticToECEF( vector<float> lla, float radius, vector<float> *ecef );
  float dist(vector<float> vec);
  double dist(vector<double> vec);
  int iDivUp(int a, int b);
};
#endif // __UTILITIES_H__
