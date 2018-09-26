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
        FORS_POSITION = 0x01, // uint16_t position, (mm): v*0.0975879
        FORS_SPEED,           // uint16_t speed, (u): (+-v/vmax)*32768 + 32768
        FORS_VIBRATE,
        FORS_START_PID,
        FORS_STOP_PID,

        FORS_RESIST_AT,
        FORS_RESIST_CLEAR,

        FORS_PID_P, // uint16_t proportional, (n/a): v/327.68
        FORS_PID_I, // uint16_t integral, (n/a): v/327.68
        FORS_PID_D, // uint16_t derivative, (n/a): v/327.68

        /// FOR COMPUTER
        // From Slider to Computer
        FORC_POSITION = 0x80, // uint16_t position, (mm): v*0.0975879
        FORC_EST_POS,         // uint16_t estimated position, (mm): v*0.0975879
        FORC_EST_VEL,         // uint16_t position, (mm/s): v*0.0975879
    };
    command_type type;
    uint16_t value = 0;

    static bool isFlag(const uint8_t ch) {
        return ch == startflag || ch == endflag || ch == escapeflag;
    }
} command;

typedef void (*bus_receiver)(command cmd);

#endif // SERIALPACKETDEFINITION_H
