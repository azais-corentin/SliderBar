#ifndef SERIALBUS_H
#define SERIALBUS_H

#include "Arduino.h"

#include "Buffer.h"
#include "SerialBusDefines.h"

class SerialBus {
public:
  SerialBus(HardwareSerial &serial);
  virtual ~SerialBus();

  bool begin(unsigned long baud);

  void set_receiver(bus_receiver r) { m_receiver = r; };

  void receive();
  void send_command(command &cmd) const;

private:
  void appendEscape(Buffer &buf, uint8_t data) const;
  void appendEscape(Buffer &buf, uint16_t data) const;

  bus_receiver m_receiver;
  HardwareSerial &m_serial;

  Buffer m_buffer;
};

#endif // SERIALBUS_H
