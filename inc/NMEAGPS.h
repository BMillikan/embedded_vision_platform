#ifndef __NMEAGPS_H__
#define __NMEAGPS_H__
#include <string>
#include <NMEAParser.h>
#include <thread>
#include <atomic>
#include <Utilities.h>
#include <BufferedAsyncSerial.h>
#include <serial_support.h>
#include <iomanip>

using namespace std;
using namespace Utilities;

///
/// \class NMEAParser
/// \brief child class of CNMEAParser which will redefine notification calls from the parent class.
///
class NMEAParser : public CNMEAParser {
public:
   NMEAParser() : CNMEAParser() { }

private:
   ///
   /// \brief This method is called whenever there is a parsing error.
   ///
   /// Redefine this method to capture errors.
   ///
   /// \param pCmd Pointer to NMEA command that caused the error. Please note that this parameter may be NULL of not completely defined. Use with caution.
   ///
   virtual void OnError(CNMEAParserData::ERROR_E nError, char *pCmd) {
      stringstream ss;
      string cmd(pCmd);

      ss << "ERROR for Cmd: " << cmd << ", Number: " << nError;
      logger.Write(ERROR, ss.str());
      //printf("ERROR for Cmd: %s, Number: %d\n", pCmd, nError);
   }

protected:
   ///
   /// \brief This method is redefined from CNMEAParserPacket::ProcessRxCommand(char *pCmd, char *pData)
   ///
   /// Here we are capturing the ProcessRxCommand to print out status. We also are looking for
   /// the GPGGA message and displaying some data from it.
   ///
   /// \param pCmd Pointer to the NMEA command string
   /// \param pData Comma separated data that belongs to the command
   /// \return Returns CNMEAParserData::ERROR_OK If successful
   ///
   virtual CNMEAParserData::ERROR_E ProcessRxCommand(char *pCmd, char *pData) {

      // Call base class to process the command
      CNMEAParser::ProcessRxCommand(pCmd, pData);
      stringstream ss;

      string cmd(pCmd);
      string data(pData);

      ss << "Cmd: " << cmd;
      logger.Write(INFO, ss.str());

      ss.str(std::string());
      ss << "Data: " << data;
      logger.Write(INFO, ss.str());
      //printf("Cmd: %s\nData: %s\n", pCmd, pData);

      // Check if this is the GPGGA command. If it is, then display some data
      if (strstr(pCmd, "GPGGA") != NULL) 
      {
         CNMEAParserData::GGA_DATA_T ggaData;
         if (GetGPGGA(ggaData) == CNMEAParserData::ERROR_OK) 
	 {
            logger.Write(INFO,"GPGGA Parsed!");
	    ss.str(std::string());
	    ss << "   Time:                " << ggaData.m_nHour << ":" << ggaData.m_nMinute << ": " << ggaData.m_nSecond;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   Latitude:            " << ggaData.m_dLatitude;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   Longitude:           " << ggaData.m_dLongitude;
            logger.Write(INFO,ss);
	    ss.str(std::string());
            ss << "   Altitude:            " << fixed << setprecision(1) << ggaData.m_dAltitudeMSL << "M";
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   GPS Quality:         " << ggaData.m_nGPSQuality;
	    logger.Write(INFO,ss);
            ss.str(std::string());
            ss << "   Satellites in view:  " << ggaData.m_nSatsInView;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   HDOP:                " << fixed << setprecision(2) << ggaData.m_dHDOP;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
            ss << "   Differential ID:     " << ggaData.m_nDifferentialID;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   Differential age:    " << ggaData.m_dDifferentialAge;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   Geoidal Separation:  " << ggaData.m_dGeoidalSep;
	    logger.Write(INFO,ss);
	    ss.str(std::string());
	    ss << "   Vertical Speed:      " << fixed << setprecision(2) << ggaData.m_dVertSpeed;
	    logger.Write(INFO,ss);
	  }
      }
      return CNMEAParserData::ERROR_OK;
    }
};


class NMEAGPS
{
public:
   NMEAGPS(void) {logger.Write(DEBUG,"NMEAGPS::NMEAGPS()");}
   virtual ~NMEAGPS() {}
protected:
   thread _listenerThread;
   virtual void _portListener();
   vector<serial_support::PortInfo> _devices_found;
   vector<serial_support::PortInfo> _enumeratePorts()
   {
      vector<serial_support::PortInfo> _devices_found = serial_support::list_ports();
      vector<serial_support::PortInfo>::iterator iter = _devices_found.begin();
      stringstream ss;
     
      logger.Write(DEBUG, "Enumerating serial ports");
      while( iter != _devices_found.end() )
      {
         serial_support::PortInfo device = *iter++;

	 ss.str(std::string());
	 ss << "(" << device.port << ", " << device.description  << ", " << device.hardware_id << ")";
	 logger.Write(INFO, ss);
      }

      return _devices_found;
   }

   void _Open(string sensorPort, int baudRate);
   void _Close(void);
private:
   NMEAParser _parser;
   BufferedAsyncSerial *_serial;

   atomic_bool _runFlag;

};
#endif
