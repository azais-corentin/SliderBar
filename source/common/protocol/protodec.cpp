#include "protodec.h"
#include "protodef.h"

namespace protocol {

void encode8(Buffer<MAX_PACKET_SIZE>& buffer, const uint8_t& data, const bool& escape)
{
    if (escape && isFlag(data)) {
        buffer.append8(escapeflag);
        buffer.append8(data ^ xorflag);
    } else
        buffer.append8(data);
}

void encode16(Buffer<MAX_PACKET_SIZE>& buffer, const uint16_t& data, const bool& escape)
{
    encode8(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode8(buffer, static_cast<uint8_t>(data), escape);
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

command decode(Buffer<MAX_PACKET_SIZE>& packet)
{
    command received;
    received.crc_valid = true;

    // Decodes the type, value, data from the data packet.
    uint8_t i = 0;
    received.type = decode8(packet, i);
    received.value = decode16(packet, i);
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