#ifndef SERIALPACKETDEFINITION_H
#define SERIALPACKETDEFINITION_H

#include <QByteArray>

struct CRC
{
    static inline uint8_t roll(uint8_t input_byte, uint8_t crc)
    {
        for (uint8_t i = 8; i; i--, input_byte >>= 1)
        {
            uint8_t result = (crc ^ input_byte) & 0x01;
            crc >>= 1;
            if (result)
                crc ^= 0x8C;
        }
        return crc;
    }

    static inline uint8_t compute(const QByteArray& input_byte, uint8_t length)
    {
        uint8_t crc = 0;
        for (uint8_t b = 0; b < length; b++)
            crc = roll(static_cast<uint8_t>(input_byte.at(b)), crc);
        return crc;
    }
};

typedef struct
{
    enum command_type : uint8_t
    {
        /// FOR SLIDER
        // From Computer to Slider
        FORS_POSITION = 0x01, // uint16_t position, v/655.36 mm
        FORS_SPEED,           // uint16_t speed, (+-v/vmax)*32768 + 32768 m/s
        FORS_VIBRATE,

        FORS_RESIST_AT,
        FORS_RESIST_CLEAR,

        /// FOR COMPUTER
        // From Slider to Computer
        FORC_POSITION = 0x80 // uint16_t position, v/655.36 mm
    };
    command_type type;
    uint16_t value;
} command;

#endif // SERIALPACKETDEFINITION_H
