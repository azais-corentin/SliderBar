#ifndef __PROTODEF_H__
#define __PROTODEF_H__

#include <cstdint>
#include <tuple>

#include <Buffer/Buffer.hpp>

namespace protocol {

const uint8_t MAX_PACKET_SIZE = 64;

extern uint8_t startflag;
extern uint8_t endflag;
extern uint8_t escapeflag;
extern uint8_t xorflag;

static inline bool isFlag(const uint8_t& ch)
{
    return ch == startflag
           || ch == endflag
           || ch == escapeflag
           || ch == xorflag;
}

static inline bool isEscape(const uint8_t& ch)
{
    return ch == escapeflag;
}

static inline std::tuple<uint8_t*, uint16_t> escape(const uint8_t* data,
                                                    const uint16_t len)
{
    Buffer<255> bufData;
    bufData.append(data, len);
}

} // namespace protocol

#endif // __PROTODEF_H__
