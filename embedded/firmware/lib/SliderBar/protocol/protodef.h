#ifndef __PROTODEF_H__
#define __PROTODEF_H__

#include <cstdint>

namespace protocol {

extern uint8_t startflag;
extern uint8_t endflag;
extern uint8_t escapeflag;
extern uint8_t xorflag;
extern uint8_t ackflag;

typedef struct {
    // Command documentation structure:
    // type name (unit) [range] = formula
    enum class command_type : uint8_t {
        /// FOR EVERYONE
        NACK = 0xFC,

        /// FOR SLIDER
        // Sent by the Computer to the Slider
        FORS_POSITION = 0x01, // uint16_t position (lsb) [0 to 2^12] = value
        FORS_VELOCITY, // int16_t velocity (lsb / s) [-2^12 to 2^12] = value
        FORS_VIBRATE, // int16_t time (ms) [0 - 2^16] = value
        /*
        FORS_START_PID,
        FORS_STOP_PID,
        */

        FORS_RESIST_AT, // int16_t position (lsb) [0 to 2^12] = value / INT16_MAX
        FORS_RESIST_CLEAR, // None

        /*
        FORS_PID_P, // float proportional (n/a) [0 to 100] = 10 * v / INT16_MAX
        FORS_PID_I, // float integral (n/a) [0 to 100] = 10 * v / INT16_MAX
        FORS_PID_D, // float derivative (n/a) [0 to 100] = 10 * v / INT16_MAX
        */

        /// FOR COMPUTER
        // Send by the Slider to the Computer
        FORC_POSITION = 0x80, // uint16_t position (lsb) [0 to 2^12] = value
        FORC_EST_POS, // uint16_t estimated_position (lsb) [0 to 2^12] = value
        FORC_EST_VEL, // int16_t estimated_velocity (lsb / s) [-2^12 to 2^12] = value
    };
    command_type type;
    int16_t value = 0;
    bool crc_valid = true;

    static bool isFlag(const uint8_t& ch)
    {
        return ch == startflag
            || ch == endflag
            || ch == escapeflag
            || ch == xorflag
            || ch == ackflag;
    }
} command;
}

#endif // __PROTODEF_H__