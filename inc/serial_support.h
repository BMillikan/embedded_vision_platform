#ifndef SERIAL_SUPPORT_H
#define SERIAL_SUPPORT_H

#include <vector>
#include <string>

namespace serial_support {

/*!
 * Structure that describes a serial device.
 */
struct PortInfo {

  /*! Address of the serial port (this can be passed to the constructor of Serial). */
  std::string port;

  /*! Human readable description of serial device if available. */
  std::string description;

  /*! Hardware ID (e.g. VID:PID of USB serial devices) or "n/a" if not available. */
  std::string hardware_id;

};

/* Lists the serial ports available on the system
 *
 * Returns a vector of available serial ports, each represented
 * by a serial::PortInfo data structure:
 *
 * \return vector of serial::PortInfo.
 */
std::vector<PortInfo>
list_ports();

} // namespace serial

#endif
