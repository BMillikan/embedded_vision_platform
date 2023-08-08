#include <Utilities.h>
#include <string>
#include <sstream>    /* std::stringstream */
#include <iostream>   /* std::cout, std::cerr */
#include <errno.h>    /* errno, strerror */
#include <cmath>
#include <fstream>
#include <serial_support.h>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Undefine "ERROR" on windows platforms
#ifdef _WIN32
#ifdef ERROR
#undef ERROR
#endif
#endif

using namespace std;

namespace Utilities
{
   void exit(const string &str)
   {
      logger.Write(ERROR, str);

      cerr << RED << str << WHT << '\n' << flush;

      std::exit(EXIT_FAILURE);
   }

   void errnoExit(const string &s)
   {
      stringstream ss;

      ss << s << " error " << errno << ", " << strerror(errno);
      logger.Write(ERROR, ss.str());

      cerr << RED << ss.str() << WHT << '\n' << flush;

      std::exit(EXIT_FAILURE);
   }

   void errnoPrint(const string &s)
   {
      stringstream ss;

      ss << s << " error " << errno << ", " << strerror(errno);
      logger.Write(ERROR, ss.str());

      cerr << RED << ss.str() << WHT << '\n' << flush;
   }

   void enumeratePorts()
   {
	vector<serial_support::PortInfo> devices_found = serial_support::list_ports();
	vector<serial_support::PortInfo>::iterator iter = devices_found.begin();

	while( iter != devices_found.end() )
	{
		serial_support::PortInfo device = *iter++;

		printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
                        device.hardware_id.c_str() );
	}
   }

   void msleep(unsigned long milliseconds) 
   {
#ifdef _WIN32
      Sleep(milliseconds); // 100 ms
#else
      usleep(milliseconds*1000); // 100 ms
#endif
   }

   bool readDataFromFile(const string& fileName, unsigned char *buffer, int length) {
      fstream myfile;

      myfile.open(fileName.c_str(), std::ios::in | std::ios::binary);

      if (myfile.is_open()) {

         myfile.read((char *)buffer, length);

         myfile.close();
      }
      else
      {
         cout << "Cannot open " << fileName << endl;
         return false;
      }

      return true;
   }

   bool readDataFromFile(const string& fileName, string &data) {
      fstream myfile;

      myfile.open(fileName.c_str(), std::ios::in);

      if (myfile.is_open()) {
         string curLine;

         while (getline(myfile, curLine)){
            data += curLine;
            if (!myfile.eof())
               data += "\n";
         }

         myfile.close();
      }
      else
         return false;

      return true;
   }

   void GeodeticToECEF(double lat, double lon, double alt, double *X, double *Y, double *Z)
   {
      stringstream ss;
      //double a = 6378137.0 / radius; // Radius at equator (WGS-84)
      double a = 1.0;
      double e2 = 6.69437999014e-3;  // First eccentricity squared (WGS-84)
      double b = sqrt(a*a*(1 - e2)); // Minor ellipsoid axis (WGS-84)

      double N = a / sqrt(1 - e2 * sin(lat) * sin(lat));


      ss << "a = " << a << ", b = " << b << ", N = " << N;
      logger.Write(INFO, ss.str());

      *X = (N + alt) * cos(lat) * cos(lon);
      *Y = (N + alt) * cos(lat) * sin(lon);
      *Z = (((b*b) / (a*a))*N + alt) * sin(lat);
   }

   void GeodeticToECEF(float lat, float lon, float alt, float *X, float *Y, float *Z)
   {
      stringstream ss;

      //double a = 6378137.0 / radius; // Radius at equator (WGS-84)
      double a = 1.0;
      double e2 = 6.69437999014e-3;  // First eccentricity squared (WGS-84)
      double b = sqrt(a*a*(1 - e2)); // Minor ellipsoid axis (WGS-84)

      double N = a / sqrt(1 - e2 * sin(lat) * sin(lat));
      
      ss << "a = " << a << ", b = " << b << ", N = " << N;
      logger.Write(INFO,ss.str());

      *X = (float)(N + alt) * cos(lat) * cos(lon);
      *Y = (float)(N + alt) * cos(lat) * sin(lon);
      *Z = (float)(((b*b) / (a*a))*N + alt) * sin(lat);
   }

   void GeodeticToECEF( vector<double> lla, double radius, vector<double> *ecef )
   {
      stringstream ss;
      double a = 6378137.0 / radius; // Radius at equator (WGS-84)
      double e2 = 6.69437999014e-3;  // First eccentricity squared (WGS-84)
      double b = sqrt(a*a*(1 - e2)); // Minor ellipsoid axis (WGS-84)

      double N = a / sqrt(1 - e2 * sin(lla[0]) * sin(lla[0]));

      ss << "a = " << a << ", b = " << b << ", N = " << N;
      logger.Write(INFO, ss.str());

      (*ecef)[0] = (N + lla[2]) * cos(lla[0]) * cos(lla[1]);
      (*ecef)[1] = (N + lla[2]) * cos(lla[0]) * sin(lla[1]);
      (*ecef)[2] = (((b*b) / (a*a))*N + lla[2]) * sin(lla[0]);
   }

   void GeodeticToECEF( vector<float> lla, float radius, vector<float> *ecef )
   {
      stringstream ss;
      double a = 6378137.0 / radius; // Radius at equator (WGS-84)
      double e2 = 6.69437999014e-3;  // First eccentricity squared (WGS-84)
      double b = sqrt(a*a*(1 - e2)); // Minor ellipsoid axis (WGS-84)

      double N = a / sqrt(1 - e2 * sin(lla[0]) * sin(lla[0]));

      ss << "a = " << a << ", b = " << b << ", N = " << N;
      logger.Write(INFO, ss.str());

      (*ecef)[0] = (float)(N + lla[2]) * cos(lla[0]) * cos(lla[1]);
      (*ecef)[1] = (float)(N + lla[2]) * cos(lla[0]) * sin(lla[1]);
      (*ecef)[2] = (float)(((b*b) / (a*a))*N + lla[2]) * sin(lla[0]);
   }

   float dist(vector<float> vec)
   {
      double ret = 0.0;

      for( auto it=vec.begin(); it != vec.end(); it++ )
         ret += *it;

      return (float)ret;
   }

   double dist(vector<double> vec)
   {
      double ret = 0.0;
      for( auto it=vec.begin(); it != vec.end(); it++ )
         ret += *it;

	  return ret;
   }

   int iDivUp(int a, int b)
   {
      return (a % b != 0) ? (a / b + 1) : (a / b);
   }
} 
