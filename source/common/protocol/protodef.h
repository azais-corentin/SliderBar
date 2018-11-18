#ifndef __PROTODEF_H__
#define __PROTODEF_H__

#include <cstdint>
#include <string>

namespace protocol {

const uint8_t MAX_PACKET_SIZE = 64;

extern uint8_t startflag;
extern uint8_t endflag;
extern uint8_t escapeflag;
extern uint8_t xorflag;
extern uint8_t ackflag;
extern uint8_t nackflag;

static inline bool isFlag(const uint8_t& ch)
{
    return ch == startflag
           || ch == endflag
           || ch == escapeflag
           || ch == xorflag
           || ch == ackflag
           || ch == nackflag;
}

static inline bool isEscape(const uint8_t& ch)
{
    return ch == escapeflag;
}

// Message topic documentation structure:
// type name (unit) [range] = formula
enum class message_topic : uint8_t {
    /// FOR EVERYONE
    NACK = 0xFC,

    /// FOR SLIDER
    // Sent by the Computer to the Slider
    FORS_POSITION = 0x01, // uint16_t position (lsb) [0 to 2^12] = value
    FORS_VELOCITY,        // int16_t velocity (lsb / s) [-2^12 to 2^12] = value
    FORS_VIBRATE,         // int16_t time (ms) [0 - 2^16] = value
    /*
    FORS_START_PID,
    FORS_STOP_PID,
    */

    FORS_RESIST_AT,    // int16_t position (lsb) [0 to 2^12] = value / INT16_MAX
    FORS_RESIST_CLEAR, // None

    /*
    FORS_PID_P, // float proportional (n/a) [0 to 100] = 10 * v / INT16_MAX
    FORS_PID_I, // float integral (n/a) [0 to 100] = 10 * v / INT16_MAX
    FORS_PID_D, // float derivative (n/a) [0 to 100] = 10 * v / INT16_MAX
    */

    /// FOR COMPUTER
    // Send by the Slider to the Computer
    FORC_POSITION = 0x80, // uint16_t position (lsb) [0 to 2^12] = value
    FORC_EST_POS,         // uint16_t estimated_position (lsb) [0 to 2^12] = value
    FORC_EST_VEL,         // int16_t estimated_velocity (lsb / s) [-2^12 to 2^12] = value
};

enum message_type : uint8_t {
    UNSIGNED_INT_8,
    UNSIGNED_INT_16,
    UNSIGNED_INT_32,
    UNSIGNED_INT_64,
    INT_8,
    INT_16,
    INT_32,
    INT_64,
    FLOAT
};

using message = struct message {
    uint8_t topic = 0;
    uint8_t type  = 0;

    // TODO: Get rid of all but one value. Maybe store the raw bytes
    // in an array and simply memcpy them to the right value type.
    uint8_t value_ui8   = 0;
    uint16_t value_ui16 = 0;
    uint32_t value_ui32 = 0;
    uint64_t value_ui64 = 0;
    int8_t value_i8     = 0;
    int16_t value_i16   = 0;
    int32_t value_i32   = 0;
    int64_t value_i64   = 0;
    float value_flt     = 0;

    bool crc_valid = false;
};
}

#endif // __PROTODEF_H__
