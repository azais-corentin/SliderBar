#include "protodec.h"

namespace protocol {

void encode8(Buffer& buffer, const uint8_t& data, bool& escape)
{
    if (escape && command::isFlag(data)) {
        buffer.append8(escapeflag);
        buffer.append8(data ^ xorflag);
    } else
        buffer.append8(data);
}

void encode16(Buffer& buffer, const uint16_t& data, bool& escape)
{
    encode8(buffer, static_cast<uint8_t>(data >> 8), escape);
    encode8(buffer, static_cast<uint8_t>(data), escape);
}

uint8_t decode8(const Buffer& buffer, uint8_t& i)
{
    if (command::isFlag(buffer.at8(i++)))
        return buffer.at8(i++) ^ xorflag;
    return buffer.at8(i - 1);
}

uint16_t decode16(const Buffer& buffer, uint8_t& i)
{
    uint8_t b1, b2;
    b1 = decode8(buffer, i);
    b2 = decode8(buffer, i);

    return static_cast<uint16_t>((b1 << 8) | (b2 & 0xff));
}

command decode(Buffer& packet)
{
    command received;
    received.crc_valid = true;

    // Decodes the type, value, data from the data packet.
    uint8_t i = 0;
    received.type = static_cast<command::command_type>(decode8(packet, i));
    uint16_t value = decode16(packet, i);
    uint8_t crc_received = decode8(packet, i);

    // Computes CRC
    Buffer receivedBytes;
    encode8(receivedBytes, static_cast<uint8_t>(received.type), false);
    encode16(receivedBytes, value, false);
    // TODO: Use hardware CRC generation
    uint8_t crc_computed = CRC8::compute(receivedBytes.data(), receivedBytes.size());

    // Compares CRC (computed vs. received)
    if (crc_computed != crc_received) {
        received.crc_valid = false;
    }

    return received;
}
}