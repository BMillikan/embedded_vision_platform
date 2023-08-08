#include <vector>
#include <serial_support.h>

using namespace std;

void enumerate_ports()
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

int main() 
{
  enumerate_ports();
}
