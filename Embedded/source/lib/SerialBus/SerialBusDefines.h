#ifndef SERIALPACKETDEFINITION_H
#define SERIALPACKETDEFINITION_H

#include <inttypes.h>

extern uint8_t startflag;
extern uint8_t endflag;
extern uint8_t escapeflag;
extern uint8_t xorflag;
extern uint8_t ackflag;

struct CRC {
    static inline uint8_t roll(uint8_t input_byte, uint8_t crc) {
        for (uint8_t i = 8; i; i--, input_byte >>= 1) {
            uint8_t result = (crc ^ input_byte) & 0x01;
            crc >>= 1;
            if (result)
                crc ^= 0x8C;
        }
        return crc;
    }

    static inline uint8_t compute(const uint8_t *input_byte, uint8_t length) {
        uint8_t crc = 0;
        for (uint8_t b = 0; b < length; b++)
            crc = roll(input_byte[b], crc);
        return crc;
    }
};

typedef struct {
    enum command_type : uint8_t {
        /// FOR EVERYONE
        NACK = 0xFC,

        /// FOR SLIDER
        // From Computer to Slider
        FORS_POSITION = 0x01, // uint16_t position, u2(mm): v/655.36 mm
        FORS_SPEED, // uint16_t speed, (mm/s)2u: (+-v/vmax)*32768 + 32768 m/s
        FORS_VIBRATE,
        FORS_START_PID,
        FORS_STOP_PID,

        FORS_RESIST_AT,
        FORS_RESIST_CLEAR,

        /// FOR COMPUTER
        // From Slider to Computer
        FORC_POSITION = 0x80 // uint16_t position, v/655.36 mm
    };
    uint8_t countFlags() {
        // Count the number of flags to escape
        uint8_t n = 0;
        if (isFlag(type))
            n++;
        if (isFlag(value & 0xff))
            n++;
        if (isFlag((value & 0xff00) >> 8))
            n++;
        return n;
    }

    command_type type;
    uint16_t value = 0;

    static bool isFlag(const uint8_t ch) {
        return ch == startflag || ch == endflag || ch == escapeflag;
    }
} command;

typedef void (*bus_receiver)(command cmd);

#endif // SERIALPACKETDEFINITION_H
