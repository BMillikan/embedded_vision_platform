#include <sstream>
#include <NMEAGPS.h>
#include <Utilities.h>
#include <sstream>
#include <vector>
#include <string>
#include <system_error>

using namespace std;
using namespace Utilities;

void NMEAGPS::_Open(string sensorPort, int baudRate)
{
    bool portFound = false;
    stringstream ss;
    _runFlag = false;

    ss << "NMEAGPS::NMEAGPS(" << sensorPort << ", " << baudRate << ")";
    logger.Write(DEBUG,ss);

    ss.str(std::string());
    ss << "Opening port ..." << sensorPort;
    logger.Write(DEBUG, ss);

    ss << "Attempting to open " << sensorPort;
    logger.Write(DEBUG,ss);
    try {
        _serial = new BufferedAsyncSerial(sensorPort, baudRate);
    } catch(boost::system::system_error& e)
    {
       ss.str(std::string());
       ss << "Cannot open port " << sensorPort;
       logger.Write(ERROR,ss);
       ss.str(std::string());
       ss << "Error: " << e.what();
       logger.Write(ERROR,ss);
       return;
    }

    ss.str(std::string());
    ss << sensorPort << " opened";
    logger.Write(DEBUG,ss);

    _runFlag = true;

    ss.str(std::string());
    ss << "Starting port " << sensorPort << " listener thread";
    logger.Write(DEBUG, ss);

    try {
       _listenerThread = std::thread(&NMEAGPS::_portListener, this); 
    }
    catch(system_error e)
    {
       ss.str(std::string());
       ss << "Unable to start port " << sensorPort << " thread";
       logger.Write(ERROR, ss);
    }

    logger.Write(DEBUG,"NMEAGPS::NMEAGPS(string,int) end");
}

void NMEAGPS::_Close()
{
    logger.Write(DEBUG, "NMEAGPS::Close()");

    // stop the listener thread
    _runFlag = false;

    logger.Write(DEBUG,"Waiting for listener thread to complete");
    _listenerThread.join();

    // delete the serial port
    delete _serial;
}

void NMEAGPS::_portListener(void)
{
   stringstream ss;
   const string delim = "\r\n";

   logger.Write(DEBUG, "NMEAGPS::_portListener()");

   _parser.Reset();

   while( _runFlag == true )
   {
      try {
         string gpsString = _serial->readStringUntil(delim);
	 _parser.ProcessNMEABuffer((char *)gpsString.data(), gpsString.size());
      }
      catch(boost::system::system_error& e)
      {
	 ss.str(std::string());
	 ss << "Error: " << e.what();
	 logger.Write(ERROR,ss);
	 break;
      }
   }      
}
