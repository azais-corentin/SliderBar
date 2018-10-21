#include "Buffer.hpp"

#include <algorithm>

template <uint8_t maxSize>
Buffer<maxSize>::Buffer()
{
    m_iBuffer = 0;
    for (int i = 0; i < maxSize; i++)
        m_buffer[i] = 0;
}

template <uint8_t maxSize>
Buffer<maxSize>::Buffer(uint8_t* data, uint8_t length)
    : Buffer()
{
    length = std::min<uint8_t>(length, maxSize);
    for (int i = 0; i < length; i++)
        m_buffer[i] = data[i];
    m_iBuffer = length;
}

template <uint8_t maxSize>
Buffer<maxSize>::~Buffer() {}

template <uint8_t maxSize>
uint8_t Buffer<maxSize>::at8(uint8_t i) const
{
    if (i >= maxSize || i < 0)
        return 0;

    return m_buffer[i];
}

template <uint8_t maxSize>
uint16_t Buffer<maxSize>::at16(uint8_t i) const
{
    if ((i + 1) >= maxSize || i < 0)
        return 0;

    return static_cast<uint16_t>((m_buffer[i] << 8) | (m_buffer[i + 1] & 0xff));
}

template <uint8_t maxSize>
void Buffer<maxSize>::clear()
{
    for (int i = 0; i < maxSize; i++)
        m_buffer[i] = 0;
    m_iBuffer = 0;
}

template <uint8_t maxSize>
bool Buffer<maxSize>::append(uint8_t* data, uint32_t len)
{
    uint8_t imax = std::min<uint8_t>(maxSize, m_iBuffer + len);
    uint8_t j = 0;
    for (uint8_t i = m_iBuffer; i < imax; i++) {
        m_buffer[i] = data[j++];
    }

    return true;
}

template <uint8_t maxSize>
bool Buffer<maxSize>::append8(uint8_t ch)
{
    if (m_iBuffer >= maxSize)
        return false; // buffer is full

    m_buffer[m_iBuffer++] = ch;
    return true;
}

template <uint8_t maxSize>
bool Buffer<maxSize>::append16(uint16_t ch)
{
    if ((m_iBuffer + 1) >= maxSize)
        return false; // buffer is full

    m_buffer[m_iBuffer++] = static_cast<uint8_t>(ch >> 8);
    m_buffer[m_iBuffer++] = static_cast<uint8_t>(ch);
    return true;
}

template <uint8_t maxSize>
bool Buffer<maxSize>::write8(uint8_t ch, uint8_t i)
{
    if (i >= maxSize || i < 0)
        return false;
    m_buffer[i++] = ch;

    m_iBuffer = std::max(i, m_iBuffer);
    return true;
}

template <uint8_t maxSize>
bool Buffer<maxSize>::write16(uint16_t ch, uint8_t i)
{
    if ((i + 1) >= maxSize || i < 0)
        return false;
    m_buffer[i++] = static_cast<uint8_t>(ch >> 8);
    m_buffer[i++] = static_cast<uint8_t>(ch);

    m_iBuffer = std::max(i, m_iBuffer);
    return true;
}

template <uint8_t maxSize>
uint8_t Buffer<maxSize>::count(uint8_t ch)
{
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < m_iBuffer; i++)
        if (m_buffer[i] == ch)
            cnt++;
    return cnt;
}

template <uint8_t maxSize>
bool Buffer<maxSize>::contains(uint8_t ch)
{
    for (uint8_t i = 0; i < m_iBuffer; i++)
        if (m_buffer[i] == ch)
            return true;
    return false;
}

template <uint8_t maxSize>
int Buffer<maxSize>::indexOf(uint8_t ch)
{
    for (uint8_t i = 0; i < maxSize; i++)
        if (m_buffer[i] == ch)
            return i;
    return -1;
}

template <uint8_t maxSize>
int Buffer<maxSize>::lastIndexOf(uint8_t ch)
{
    for (uint8_t i = m_iBuffer - 1; i >= 0; i--)
        if (m_buffer[i] == ch)
            return i;
    return -1;
}

template <uint8_t maxSize>
Buffer<maxSize> Buffer<maxSize>::mid(uint8_t position, uint8_t length)
{
    if (length == -1)
        return Buffer(m_buffer + position, size() - position);
    if (position <= size())
        return Buffer(m_buffer + position, length);
    else
        return Buffer();
}

template <uint8_t maxSize>
Buffer<maxSize> Buffer<maxSize>::left(uint8_t length)
{
    return Buffer(m_buffer, length);
}

template <uint8_t maxSize>
void Buffer<maxSize>::chop(uint8_t n)
{
    uint8_t length = size() - n;
    for (uint8_t i = length; i < size(); i++)
        m_buffer[i] = 0;
    m_iBuffer = length;
}
