#ifndef BUFFER_H
#define BUFFER_H

#include "Arduino.h"

#define BUFFER_SIZE 8

class Buffer {
public:
  Buffer();
  virtual ~Buffer();

  uint8_t *data() { return m_buffer; };

  uint8_t at(int i);
  uint16_t at16(int i);
  void clear();
  bool append(uint8_t ch);
  bool append16(uint16_t ch);
  bool write(uint8_t ch, int i);
  bool write16(uint16_t ch, int i);

  uint8_t count(uint8_t ch);
  bool contains(uint8_t ch) { return count(ch) > 0; };

private:
  uint8_t m_buffer[BUFFER_SIZE];
  int m_iBuffer = 0;
};

#endif // BUFFER_H
