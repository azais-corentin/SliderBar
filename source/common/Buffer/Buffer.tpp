#include "Buffer.hpp"

#include <algorithm>
// For std::memcpy
#include <cstring>

template <uint8_t N>
Buffer<N>::Buffer()
{
    index = 0;
    /*
    for (int i = 0; i < N; i++)
        buffer[i] = 0;
    */
}

template <uint8_t N>
Buffer<N>::Buffer(uint8_t* _data, uint8_t length)
{
    length = std::min<uint8_t>(length, N);
    for (int i = 0; i < length; i++)
        buffer[i] = _data[i];
    index = length;
}

template <uint8_t N>
Buffer<N> Buffer<N>::extract(uint8_t iBegin, uint8_t iEnd)
{
    if (iBegin >= iEnd)
        return Buffer();

    if (iEnd >= size())
        iEnd = size() - 1;

    return Buffer(data(iBegin), iEnd - iBegin);
}

template <uint8_t N>
uint8_t Buffer<N>::at8(uint8_t i) const
{
    if (i >= index || i < 0)
        return 0;

    return buffer[i];
}

template <uint8_t N>
uint16_t Buffer<N>::at16(uint8_t i) const
{
    // Check if out of bounds
    if (i < 0 || (i + 1) >= index)
        return 0;

    return static_cast<uint16_t>((buffer[i] << 8) | buffer[i + 1]);
}

template <uint8_t N>
int8_t Buffer<N>::at8_signed(uint8_t i) const
{
    if (i >= index || i < 0)
        return 0;

    int8_t ret;
    // Bit by bit copy to decode
    memcpy(&ret, &buffer[i], sizeof(ret));

    return ret;
}

template <uint8_t N>
void Buffer<N>::clear()
{
    index = 0;
}

template <uint8_t N>
bool Buffer<N>::append(const uint8_t* data, const uint8_t& len)
{
    if (index >= N)
        return false;

    if (index + len > N) {
        uint8_t reallen = std::min<uint8_t>(len, N - index);

        std::memcpy(&buffer[index], data, reallen);
        index += reallen;
        return false;
    }

    std::memcpy(&buffer[index], data, len);
    index += len;
    return true;
}

template <uint8_t N>
bool Buffer<N>::append(const Buffer<N>& data)
{
    if (index >= N)
        return false;

    if (index + data.size() > N) {
        uint8_t reallen = std::min<uint8_t>(data.size(), N - index);

        std::memcpy(buffer + index, data.data(), reallen);

        return false;
    }

    std::memcpy(&buffer[index], data.data(), data.size());

    return true;
}

template <uint8_t N>
bool Buffer<N>::append(const uint8_t& ch)
{
    if (index >= N)
        return false; // buffer is full

    buffer[index++] = ch;
    return true;
}

template <uint8_t N>
bool Buffer<N>::append(const uint16_t& ch)
{
    if ((index + 1) >= N)
        return false; // buffer is full

    buffer[index++] = static_cast<uint8_t>(ch >> 8);
    buffer[index++] = static_cast<uint8_t>(ch);
    return true;
}

template <uint8_t N>
bool Buffer<N>::write(const uint8_t ch, uint8_t i)
{
    if (i >= N || i < 0)
        return false;
    buffer[i++] = ch;

    index = std::max(i, index);
    return true;
}

template <uint8_t N>
bool Buffer<N>::write(const uint16_t ch, uint8_t i)
{
    if ((i + 1) >= N || i < 0)
        return false;
    buffer[i++] = static_cast<uint8_t>(ch >> 8);
    buffer[i++] = static_cast<uint8_t>(ch);

    index = std::max(i, index);
    return true;
}

template <uint8_t N>
bool Buffer<N>::write(const uint32_t ch, uint8_t i)
{
    if ((i + 2) >= N || i < 0)
        return false;

    buffer[i++] = static_cast<uint8_t>(ch >> 16);
    buffer[i++] = static_cast<uint8_t>(ch >> 8);
    buffer[i++] = static_cast<uint8_t>(ch);

    index = std::max(i, index);
    return true;
}

template <uint8_t N>
uint8_t Buffer<N>::count(uint8_t ch)
{
    uint8_t cnt = 0;
    for (uint8_t i = 0; i < index; i++)
        if (buffer[i] == ch)
            cnt++;
    return cnt;
}

template <uint8_t N>
bool Buffer<N>::contains(uint8_t ch)
{
    for (uint8_t i = 0; i < index; i++)
        if (buffer[i] == ch)
            return true;
    return false;
}

template <uint8_t N>
int Buffer<N>::indexOf(uint8_t ch)
{
    for (uint8_t i = 0; i < index; i++)
        if (buffer[i] == ch)
            return i;
    return -1;
}

template <uint8_t N>
int Buffer<N>::lastIndexOf(uint8_t ch)
{
    for (uint8_t i = index - 1; i >= 0; i--)
        if (buffer[i] == ch)
            return i;
    return -1;
}

template <uint8_t N>
void Buffer<N>::mid(uint8_t position, uint8_t length)
{
    // There's no data to copy, simple empty the array
    if (position >= index) {
        index = 0;
        return;
    }

    // Length would encompass elements out of bounds, calculate a new length
    if (position + length > N)
        length = N - position;

    // Move the elements in range [position, position + length] to the front
    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = buffer[i + position];
    }
    // Discards the other elements
    index = length;
}

template <uint8_t N>
void Buffer<N>::chop(uint8_t n)
{
    index -= n;
}
