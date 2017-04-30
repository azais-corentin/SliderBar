#ifndef SERIALPACKETDEFINITION_H
#define SERIALPACKETDEFINITION_H

#include <QByteArray>

typedef struct
{
    enum command_type : uint8_t
    {
        // From Computer to Slider
        FORS_POSITION = 0x01,
        FORS_SPEED = 0x02,
        FORS_VIBRATE = 0x03,

        FORS_RESIST_AT = 0x04,
        FORS_RESIST_CLEAR = 0x05,

        // From Slider to Computer
        FORC_POSITION = 0x80
    };
    command_type type;
    uint16_t value;
} command;

struct encode
{
    union
    {
        uint8_t u8;
        uint16_t u16;
    };
    QByteArray m_packet;
    int& m_i;
    encode(const QByteArray& packet, int& i) : m_packet(packet), m_i(i) {}
    operator uint8_t() { return 8; }
    operator uint16_t() { return 16; }
    operator uint32_t() { return 32; }
};

struct decode
{
    QByteArray m_packet;
    int& m_i;
    decode(const QByteArray& packet, int& i) : m_packet(packet), m_i(i) {}
    operator uint8_t()
    {
        return static_cast<uint8_t>(m_packet.at(m_i++));
    }
    operator uint16_t()
    {
        m_i += 2;
        return static_cast<uint16_t>(m_packet.at(m_i - 2) << 8 | m_packet.at(m_i - 1));
    }

    operator uint32_t() { return 32; }
};

#endif // SERIALPACKETDEFINITION_H
