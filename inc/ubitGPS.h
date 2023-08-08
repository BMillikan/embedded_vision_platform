#ifndef __UBITGPS_H__
#define __UBITGPS_H__

#include <NMEAGPS.h>
#include <string>
#include <Utilities.h>
#include <sstream>

using namespace std;
using namespace Utilities;

class ubitGPS : public NMEAGPS
{
public:

   ubitGPS(void)
   {
      logger.Write(DEBUG, "ubitGPS::ubitGPS() begin");

      _devices_found = _enumeratePorts();
      string port;
      bool portFound = false;
      stringstream ss;

      vector<serial_support::PortInfo>::iterator iter = _devices_found.begin();

      logger.Write(INFO, "Searching for ubit GPS port");

      while( iter != _devices_found.end() )
      {
         serial_support::PortInfo device = *iter++;

	 logger.Write(DEBUG, device.description);
	 if( device.description.find("u-blox 7 - GPS/GNSS") != string::npos )
	 {
	    ss.str(std::string());

	    ss << "Found ubit GPS at " << device.port;
	    logger.Write(INFO,ss);
            port = device.port;
	    portFound = true;
	    break;
	 }
      }

      if( portFound )
         _Open(port, 9600);
      else
	 logger.Write(ERROR,"ubit GPS not found");

      logger.Write(DEBUG, "ubitGPS::ubitGPS() end");
   }

   ~ubitGPS(void)
   {
      _Close();
   }

};
#endif // __UBITGPS_H__
