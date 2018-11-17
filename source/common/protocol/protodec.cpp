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

uint16_t decode16(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    uint8_t b1, b2;
    b1 = decode8(buffer, i);
    b2 = decode8(buffer, i);

    return static_cast<uint16_t>((b1 << 8) | (b2 & 0xff));
}

uint8_t decodeu8(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    if (isFlag(buffer.at8(i++)))
        return buffer.at8(i++) ^ xorflag;
    return buffer.at8(i - 1);
}

uint16_t decodeu16(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    uint8_t bits[2];
    bits[0] = decode8(buffer, i);
    bits[1] = decode8(buffer, i);

    return static_cast<uint16_t>((bits[0] << 8) | (bits[1]));
}

uint32_t decodeu32(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    uint8_t bits[4];
    bits[0] = decode8(buffer, i);
    bits[1] = decode8(buffer, i);
    bits[2] = decode8(buffer, i);
    bits[3] = decode8(buffer, i);

    return static_cast<uint16_t>((bits[0] << 24) | (bits[1] << 16) | (bits[2] << 8) | (bits[3]));
}

uint64_t decodeu64(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
    uint8_t bits[8];
    bits[0] = decode8(buffer, i);
    bits[1] = decode8(buffer, i);
    bits[2] = decode8(buffer, i);
    bits[3] = decode8(buffer, i);
    bits[4] = decode8(buffer, i);
    bits[5] = decode8(buffer, i);
    bits[6] = decode8(buffer, i);
    bits[7] = decode8(buffer, i);

    return static_cast<uint16_t>((bits[0] << 56) | (bits[1] << 48) | (bits[2] << 40) | (bits[3] << 32)
                                 | (bits[4] << 24) | (bits[5] << 16) | (bits[6] << 8) | (bits[7]));
}

int8_t decode8(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
}

int16_t decode16(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
}

int32_t decode32(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
}

int64_t decode64(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
}

float decodefloat(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
}

std::string decodestring(const Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
{
}

message decode(Buffer<MAX_PACKET_SIZE>& packet)
{
    message received;
    received.crc_valid = true;

    // Decodes the type, value, data from the data packet.
    uint8_t i      = 0;
    received.topic = decode8(packet, i);
    received.type  = decode8(packet, i);

    switch (received.type) {
    case value:

        break;
    default:
        break;
    }

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
