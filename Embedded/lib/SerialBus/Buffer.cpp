#include "Buffer.h"

Buffer::Buffer() {
  for (int i = 0; i < BUFFER_SIZE; i++)
    m_buffer[i] = 0;
}

Buffer::~Buffer() {}

uint8_t Buffer::at(int i) {
  if (i >= BUFFER_SIZE || i < 0)
    return 0;

  return m_buffer[i];
}

uint16_t Buffer::at16(int i) {
  if ((i + 1) >= BUFFER_SIZE || i < 0)
    return 0;

  return static_cast<uint16_t>(m_buffer[i] << 8 | m_buffer[i + 1]);
}

void Buffer::clear() {
  for (int i = 0; i < BUFFER_SIZE; i++)
    m_buffer[i] = 0;
  m_iBuffer = 0;
}

bool Buffer::append(uint8_t ch) {
  if (m_iBuffer >= BUFFER_SIZE)
    return false; // buffer is full

  m_buffer[m_iBuffer++] = ch;
  return true;
}

bool Buffer::append16(uint16_t ch) {
  if ((m_iBuffer + 1) >= BUFFER_SIZE)
    return false; // buffer is full

  m_buffer[m_iBuffer++] = static_cast<uint8_t>(ch >> 8);
  m_buffer[m_iBuffer++] = static_cast<uint8_t>(ch);
  return true;
}

bool Buffer::write(uint8_t ch, int i) {
  if (i >= BUFFER_SIZE || i < 0)
    return false;
  m_buffer[i++] = ch;

  m_iBuffer = max(i, m_iBuffer);
  return true;
}

bool Buffer::write16(uint16_t ch, int i) {
  if ((i + 1) >= BUFFER_SIZE || i < 0)
    return false;
  m_buffer[i++] = static_cast<uint8_t>(ch >> 8);
  m_buffer[i++] = static_cast<uint8_t>(ch);

  m_iBuffer = max(i, m_iBuffer);
  return true;
}

uint8_t Buffer::count(uint8_t ch) {
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < BUFFER_SIZE; i++)
    if (m_buffer[i] == ch)
      cnt++;
  return cnt;
}
