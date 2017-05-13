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
    void encode8(Buffer &packet, uint8_t data, bool escape = true);
    void encode16(Buffer &packet, uint16_t data, bool escape = true);

    uint8_t decode8(const Buffer &packet, int &i);
    uint16_t decode16(const Buffer &packet, int &i);

    bus_receiver m_receiver;
    HardwareSerial &m_serial;

    Buffer m_buffer;
};

#endif // SERIALBUS_H
