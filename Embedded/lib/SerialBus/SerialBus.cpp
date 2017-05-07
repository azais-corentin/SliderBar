#include "SerialBus.h"

uint8_t startflag = 0x12;
uint8_t endflag = 0x13;
uint8_t escapeflag = 0x7D;
uint8_t xorflag = 0x20;

SerialBus::SerialBus(HardwareSerial &serial) : m_serial(serial) {}

SerialBus::~SerialBus() {}

bool SerialBus::begin(unsigned long baud) {
  m_serial.begin(baud);
  return true;
}

void SerialBus::send_command(command &cmd) const {
  // Send the command over serial
  const uint8_t length = 6 + cmd.countFlags();
  Buffer buf;

  // Append startflag
  buf.append(startflag);

  // Append length
  appendEscape(buf, length);

  // Append command
  appendEscape(buf, static_cast<uint8_t>(cmd.type));
  appendEscape(buf, cmd.value);

  // Append endflag
  buf.append(endflag);

  m_serial.write(buf.data(), length);
}

void SerialBus::receive() {
  if (!m_serial.available() > 0)
    return;

  uint8_t rcByte;
  while (m_serial.available() > 0) {
    rcByte = m_serial.read();
    // m_buffer[m_iBuffer++] = rcByte;
  }
}

void SerialBus::appendEscape(Buffer &buf, uint8_t data) const {
  if (command::isFlag(data)) {
    buf.append(escapeflag);
    buf.append(data ^ xorflag);
  } else
    buf.append(data);
}

void SerialBus::appendEscape(Buffer &buf, uint16_t data) const {
  // Append first byte
  if (command::isFlag(static_cast<uint8_t>(data >> 8))) {
    buf.append(escapeflag);
    buf.append(static_cast<uint8_t>(data >> 8) ^ xorflag);
  } else
    buf.append(static_cast<uint8_t>(data >> 8));

  // Append second byte
  if (command::isFlag(static_cast<uint8_t>(data))) {
    buf.append(escapeflag);
    buf.append(static_cast<uint8_t>(data) ^ xorflag);
  } else
    buf.append(static_cast<uint8_t>(data));
}
