#ifndef PROTOCOL_STRUCTURES_H
#define PROTOCOL_STRUCTURES_H

#include <cstdint>

namespace protocol {

struct CalibrationData {
    uint16_t minimumPosition = 0;
    uint16_t maximumPosition = 1023;
    uint16_t minimumVelocity = 255;
    uint16_t maximumVeloicty = 5110;
};

}

#endif // PROTOCOL_STRUCTURES_H
