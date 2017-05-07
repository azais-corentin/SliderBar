#ifndef SERIALBUS_H
#define SERIALBUS_H

#include "Arduino.h"

#include "Buffer.h"
#include "SerialBusDefines.h"

class SerialBus {
public:
  SerialBus(HardwareSerial &serial, unsigned long baud);
  virtual ~SerialBus();

  void set_receiver(bus_receiver r) { m_receiver = r; };

  void receive();
  void send_command(command &cmd) const;

private:
  bus_receiver m_receiver;
  HardwareSerial &m_serial;

  Buffer m_buffer;
};

#endif // SERIALBUS_H
