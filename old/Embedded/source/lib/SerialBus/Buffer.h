#ifndef BUFFER_H
#define BUFFER_H

#include "Arduino.h"

// startflag + 3 escape + 3 data + escape + crc + endflag
#define MAX_BUFFER_SIZE 10

class Buffer {
  public:
    Buffer();
    Buffer(uint8_t *data, int length);
    virtual ~Buffer();

    uint8_t *data() { return m_buffer; }

    uint8_t at8(int i) const;
    uint16_t at16(int i) const;
    void clear();
    bool append8(uint8_t ch);
    bool append16(uint16_t ch);
    bool write8(uint8_t ch, int i);
    bool write16(uint16_t ch, int i);

    uint8_t size() { return m_iBuffer; };
    uint8_t count(uint8_t ch);
    bool contains(uint8_t ch);

    int indexOf(uint8_t ch);
    int lastIndexOf(uint8_t ch);
    Buffer mid(uint8_t position, int length = -1);
    Buffer left(uint8_t length);
    void chop(uint8_t n);

  private:
    uint8_t m_buffer[MAX_BUFFER_SIZE];
    int m_iBuffer;
};

#endif // BUFFER_H
