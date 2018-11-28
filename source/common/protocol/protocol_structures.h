#ifndef PROTOCOL_STRUCTURES_H
#define PROTOCOL_STRUCTURES_H

#include <cstdint>

namespace protocol {

struct calibrationData {
    uint16_t minimumPosition;
    uint16_t maximumPosition;
    uint16_t minimumVelocity;
    uint16_t maximumVeloicty;
};

}

#endif // PROTOCOL_STRUCTURES_H
