#ifndef __PROTODEF_H__
#define __PROTODEF_H__

#include <cstdint>

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

} // namespace protocol

#endif // __PROTODEF_H__