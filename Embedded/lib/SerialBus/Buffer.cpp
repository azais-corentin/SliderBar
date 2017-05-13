#include "Buffer.h"

Buffer::Buffer() {
    m_iBuffer = 0;
    for (int i = 0; i < MAX_BUFFER_SIZE; i++)
        m_buffer[i] = 0;
}

Buffer::Buffer(uint8_t *data, int length) : Buffer() {
    length = min(length, MAX_BUFFER_SIZE);
    for (int i = 0; i < length; i++)
        m_buffer[i] = data[i];
    m_iBuffer = length;
}

Buffer::~Buffer() {}

uint8_t Buffer::at8(int i) const {
    if (i >= MAX_BUFFER_SIZE || i < 0)
        return 0;

    return m_buffer[i];
}

uint16_t Buffer::at16(int i) const {
    if ((i + 1) >= MAX_BUFFER_SIZE || i < 0)
        return 0;

    return static_cast<uint16_t>((m_buffer[i] << 8) | (m_buffer[i + 1] & 0xff));
}

void Buffer::clear() {
    for (int i = 0; i < MAX_BUFFER_SIZE; i++)
        m_buffer[i] = 0;
    m_iBuffer = 0;
}

bool Buffer::append(uint8_t ch) {
    if (m_iBuffer >= MAX_BUFFER_SIZE)
        return false; // buffer is full

    m_buffer[m_iBuffer++] = ch;
    return true;
}

bool Buffer::append16(uint16_t ch) {
    if ((m_iBuffer + 1) >= MAX_BUFFER_SIZE)
        return false; // buffer is full

    m_buffer[m_iBuffer++] = static_cast<uint8_t>(ch >> 8);
    m_buffer[m_iBuffer++] = static_cast<uint8_t>(ch);
    return true;
}

bool Buffer::write(uint8_t ch, int i) {
    if (i >= MAX_BUFFER_SIZE || i < 0)
        return false;
    m_buffer[i++] = ch;

    m_iBuffer = max(i, m_iBuffer);
    return true;
}

bool Buffer::write16(uint16_t ch, int i) {
    if ((i + 1) >= MAX_BUFFER_SIZE || i < 0)
        return false;
    m_buffer[i++] = static_cast<uint8_t>(ch >> 8);
    m_buffer[i++] = static_cast<uint8_t>(ch);

    m_iBuffer = max(i, m_iBuffer);
    return true;
}

uint8_t Buffer::count(uint8_t ch) {
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < MAX_BUFFER_SIZE; i++)
        if (m_buffer[i] == ch)
            cnt++;
    return cnt;
}

int Buffer::indexOf(uint8_t ch) {
    for (uint8_t i = 0; i < MAX_BUFFER_SIZE; i++)
        if (m_buffer[i] == ch)
            return i;
    return -1;
}

int Buffer::lastIndexOf(uint8_t ch) {
    for (uint8_t i = MAX_BUFFER_SIZE - 1; i >= 0; i--)
        if (m_buffer[i] == ch)
            return i;
    return -1;
}

Buffer Buffer::mid(uint8_t position, int length) {
    if (length == -1)
        return Buffer(m_buffer + position, size() - position);
    return Buffer(m_buffer + position, length);
}

Buffer Buffer::left(uint8_t length) { return Buffer(m_buffer, length); }

void Buffer::chop(uint8_t n) {
    uint8_t length = size() - n;
    for (uint8_t i = length; i < size(); i++)
        m_buffer[i] = 0;
    m_iBuffer = length;
}
