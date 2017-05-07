#include "SerialBus.h"

uint8_t startflag = 0x12;
uint8_t endflag = 0x13;
uint8_t escapeflag = 0x7D;
uint8_t xorflag = 0x20;

SerialBus::SerialBus(HardwareSerial &serial, unsigned long baud)
    : m_serial(serial) {
  m_serial.begin(baud);
}

SerialBus::~SerialBus() {}

void SerialBus::send_command(command &cmd) const {
  // Send the command over serial
  const uint8_t length = 5 + cmd.countFlags();
  Buffer buf;

  // Append startflag
  buf.append(startflag);

  // Append type
  if (command::isFlag(cmd.type)) {
    buf.append(escapeflag);
    buf.append(cmd.type ^ xorflag);
  } else
    buf.append(cmd.type);
  uint8_t byte1 = static_cast<uint8_t>(cmd.value >> 8);
  uint8_t byte2 = static_cast<uint8_t>(cmd.value);

  // Append byte1
  if (command::isFlag(byte1)) {
    buf.append(escapeflag);
    buf.append(byte1 ^ xorflag);
  } else
    buf.append(byte1);

  // Append byte2
  if (command::isFlag(byte2)) {
    buf.append(escapeflag);
    buf.append(byte2 ^ xorflag);
  } else
    buf.append(byte2);

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
