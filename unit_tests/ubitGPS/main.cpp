#include <boost/date_time/posix_time/posix_time.hpp>
#include <ubitGPS.h>
#include <iostream>
#include <Utilities.h>

using namespace std;
using namespace Utilities;

int main()
{
   logger.Write(DEBUG, "Creating ubit GPS object...");
   ubitGPS *gps = new ubitGPS;

   cout << "Press [ENTER] to exit..." << endl;

   char a = getchar();

   logger.Write(DEBUG, "Deleting GPS object...");
   delete gps;
}  
