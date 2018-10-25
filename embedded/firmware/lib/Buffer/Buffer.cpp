#include "Buffer.hpp"

#include <algorithm>
// For std::memcpy
#include <cstring>

Buffer::Buffer()
{
    index = 0;
    /*
    for (int i = 0; i < MAX_PACKET_SIZE; i++)
        buffer[i] = 0;
    */
}

Buffer::Buffer(uint8_t* _buffer, uint8_t length)
{
    length = std::min<uint8_t>(length, MAX_PACKET_SIZE);
    for (int i = 0; i < length; i++)
        buffer[i] = buffer[i];
    index = length;
}

Buffer::~Buffer()
{
}

uint8_t Buffer::at8(uint8_t i) const
{
    if (i >= index || i < 0)
        return 0;

    return buffer[i];
}

uint16_t Buffer::at16(uint8_t i) const
{
    // Check if out of bounds
    if (i < 0 || (i + 1) >= index)
        return 0;

    return static_cast<uint16_t>((buffer[i] << 8) | buffer[i + 1]);
}

void Buffer::clear()
{
    index = 0;
}

bool Buffer::append(uint8_t* data, uint8_t len)
{
    if (index >= MAX_PACKET_SIZE)
        return false;
    else if (index + len > MAX_PACKET_SIZE) {
        uint8_t reallen = std::min<uint8_t>(len, MAX_PACKET_SIZE - index);

        std::memcpy(&buffer[index], data, reallen);
        return false;
    }

    std::memcpy(&buffer[index], data, len);
    return true;
}

bool Buffer::append8(uint8_t ch)
{
    if (index >= MAX_PACKET_SIZE)
        return false; // buffer is full

    buffer[index++] = ch;
    return true;
}

bool Buffer::append16(uint16_t ch)
{
    if ((index + 1) >= MAX_PACKET_SIZE)
        return false; // buffer is full

    buffer[index++] = static_cast<uint8_t>(ch >> 8);
    buffer[index++] = static_cast<uint8_t>(ch);
    return true;
}

bool Buffer::write8(uint8_t ch, uint8_t i)
{
    if (i >= MAX_PACKET_SIZE || i < 0)
        return false;
    buffer[i++] = ch;

    index = std::max(i, index);
    return true;
}

bool Buffer::write16(uint16_t ch, uint8_t i)
{
    if ((i + 1) >= MAX_PACKET_SIZE || i < 0)
        return false;
    buffer[i++] = static_cast<uint8_t>(ch >> 8);
    buffer[i++] = static_cast<uint8_t>(ch);

    index = std::max(i, index);
    return true;
}

uint8_t Buffer::count(uint8_t ch)
{
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < index; i++)
        if (buffer[i] == ch)
            cnt++;
    return cnt;
}

bool Buffer::contains(uint8_t ch)
{
    for (uint8_t i = 0; i < index; i++)
        if (buffer[i] == ch)
            return true;
    return false;
}

int Buffer::indexOf(uint8_t ch)
{
    for (uint8_t i = 0; i < index; i++)
        if (buffer[i] == ch)
            return i;
    return -1;
}

int Buffer::lastIndexOf(uint8_t ch)
{
    for (uint8_t i = index - 1; i >= 0; i--)
        if (buffer[i] == ch)
            return i;
    return -1;
}

void Buffer::mid(uint8_t position, uint8_t length)
{
    // There's no data to copy, simple empty the array.
    if (position >= index) {
        index = 0;
        return;
    }

    // Length would encompass elements out of bounds Calculate a new length.
    if (position + length > MAX_PACKET_SIZE)
        length = MAX_PACKET_SIZE - position;

    // Move the elements in range [position, position + length] to the front and
    // discards the other elements.
    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = buffer[i + position];
    }
    index = length;
}

void Buffer::chop(uint8_t n)
{
    index -= n;
}
