#ifndef SERIALPACKETDEFINITION_H
#define SERIALPACKETDEFINITION_H

#include <QByteArray>

typedef struct
{
    enum command_type : uint8_t
    {
        /// FOR SLIDER
        // From Computer to Slider
        FORS_POSITION = 0x01, // uint16_t position, v*655.36mm
        FORS_SPEED,           // uint16_t speed, m/s
        FORS_VIBRATE,

        FORS_RESIST_AT,
        FORS_RESIST_CLEAR,

        /// FOR COMPUTER
        // From Slider to Computer
        FORC_POSITION = 0x80 // uint16_t position, v*655.36mm
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
        return static_cast<uint16_t>((m_packet.at(m_i - 2) << 8) | (m_packet.at(m_i - 1) & 0xff));
    }
};

#endif // SERIALPACKETDEFINITION_H
