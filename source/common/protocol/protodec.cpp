#include "protodec.h"
#include "protodef.h"

namespace protocol {

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const uint8_t& data, const bool& escape)
{
    if (escape && isFlag(data)) {
        buffer.append(escapeflag);
        buffer.append(static_cast<uint8_t>(data ^ xorflag));
    } else
        buffer.append(data);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const uint16_t& data, const bool& escape)
{
    encode(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode(buffer, static_cast<uint8_t>(data), escape);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const uint32_t& data, const bool& escape)
{
    encode(buffer, static_cast<uint8_t>(data >> 24), escape);
    encode(buffer, static_cast<uint8_t>(data >> 16), escape);
    encode(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode(buffer, static_cast<uint8_t>(data), escape);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const uint64_t& data, const bool& escape)
{
    encode(buffer, static_cast<uint8_t>(data >> 56), escape);
    encode(buffer, static_cast<uint8_t>(data >> 48), escape);
    encode(buffer, static_cast<uint8_t>(data >> 40), escape);
    encode(buffer, static_cast<uint8_t>(data >> 32), escape);
    encode(buffer, static_cast<uint8_t>(data >> 24), escape);
    encode(buffer, static_cast<uint8_t>(data >> 16), escape);
    encode(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode(buffer, static_cast<uint8_t>(data), escape);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const int8_t& data, const bool& escape)
{
    if (escape && isFlag(data)) {
        buffer.append(escapeflag);
        buffer.append(static_cast<int8_t>(data ^ xorflag));
    } else
        buffer.append(data);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const int16_t& data, const bool& escape)
{
    encode(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode(buffer, static_cast<uint8_t>(data), escape);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const int32_t& data, const bool& escape)
{
    encode(buffer, static_cast<uint8_t>(data >> 24), escape);
    encode(buffer, static_cast<uint8_t>(data >> 16), escape);
    encode(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode(buffer, static_cast<uint8_t>(data), escape);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const int64_t& data, const bool& escape)
{
    encode(buffer, static_cast<uint8_t>(data >> 56), escape);
    encode(buffer, static_cast<uint8_t>(data >> 48), escape);
    encode(buffer, static_cast<uint8_t>(data >> 40), escape);
    encode(buffer, static_cast<uint8_t>(data >> 32), escape);
    encode(buffer, static_cast<uint8_t>(data >> 24), escape);
    encode(buffer, static_cast<uint8_t>(data >> 16), escape);
    encode(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode(buffer, static_cast<uint8_t>(data), escape);
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const float& data, const bool& escape)
{
}

void encode(Buffer<MAX_PACKET_SIZE>& buffer, const std::string& data, const bool& escape)
{
}

uint8_t decode8(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    if (isFlag(buffer.at8(i++)))
        return buffer.at8(i++) ^ xorflag;
    return buffer.at8(i - 1);
}

uint16_t decode16(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    uint8_t b1, b2;
    b1 = decode8(buffer, i);
    b2 = decode8(buffer, i);

    return static_cast<uint16_t>((b1 << 8) | (b2 & 0xff));
}

message decode(Buffer<MAX_PACKET_SIZE>& packet)
{
    message received;
    received.crc_valid = true;

    // Decodes the type, value, data from the data packet.
    uint8_t i            = 0;
    received.type        = decode8(packet, i);
    received.value       = decode16(packet, i);
    uint8_t crc_received = decode8(packet, i);

    // Computes CRC
    Buffer<MAX_PACKET_SIZE> receivedBytes;
    encode8(receivedBytes, received.type, false);
    encode16(receivedBytes, received.value, false);
    // TODO: Use hardware CRC generation
    uint8_t crc_computed = CRC8::compute(receivedBytes.data(), receivedBytes.size());

    // Compares CRC (computed vs. received)
    if (crc_computed != crc_received) {
        received.crc_valid = false;
    }

    return received;
}
} // namespace protocol
