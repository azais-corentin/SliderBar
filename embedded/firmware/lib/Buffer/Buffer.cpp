#include "Buffer.hpp"

#include <algorithm>
// For std::memcpy
#include <cstrin>

Buffer::Buffer()
{
    index = 0;
    for (int i = 0; i < MAX_PACKET_SIZE; i++)
        buffer[i] = 0;
}

Buffer::Buffer(uint8_t* _buffer, uint8_t length)
    : Buffer()
{
    length = std::min<uint8_t>(length, MAX_PACKET_SIZE);
    for (int i = 0; i < length; i++)
        buffer[i] = buffer[i];
    index = length;
}

Buffer::Buffer(sbarray _buffer)
{
    buffer = _buffer;
    index = buffer.size();
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
    if ((i + 1) >= MAX_PACKET_SIZE || i < 0 || (i + 2) >= index)
        return 0;

    return static_cast<uint16_t>((buffer[i] << 8) | (buffer[i + 1] & 0xff));
}

void Buffer::clear()
{
    for (int i = 0; i < MAX_PACKET_SIZE; i++)
        buffer[i] = 0;
    index = 0;
}

bool Buffer::append(uint8_t* data, uint8_t len)
{
    if (index >= MAX_PACKET_SIZE)
        return false;
    else if (len >= MAX_PACKET_SIZE - index) {
        uint8_t reallen = std::min(len, static_cast<uint8_t>(MAX_PACKET_SIZE - index));

        std::memcpy(buffer, data, reallen);
        return false;
    }

    std::memcpy(buffer, data, len);

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
    for (uint8_t i = 0; i < MAX_PACKET_SIZE; i++)
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

Buffer Buffer::mid(uint8_t position, uint8_t length)
{
    if (length == -1)
        return Buffer(&buffer[0] + position, size() - position);
    if (position <= size())
        return Buffer(&buffer[0] + position, length);
    else
        return Buffer();
}

void Buffer::chop(uint8_t n)
{
    uint8_t length = size() - n;
    for (uint8_t i = length; i < size(); i++)
        buffer[i] = 0;
    index = length;
}
