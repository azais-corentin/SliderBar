#ifndef SERIALPACKETDEFINITION_H
#define SERIALPACKETDEFINITION_H

#include <inttypes.h>

extern uint8_t startflag;
extern uint8_t endflag;
extern uint8_t escapeflag;
extern uint8_t xorflag;

typedef struct {
  enum command_type : uint8_t {
    /// FOR SLIDER
    // From Computer to Slider
    FORS_POSITION = 0x01, // uint16_t position, 655536/100m
    FORS_SPEED = 0x02,    // uint16_t speed, m/s
    FORS_VIBRATE = 0x03,

    FORS_RESIST_AT = 0x04,
    FORS_RESIST_CLEAR = 0x05,

    /// FOR COMPUTER
    // From Slider to Computer
    FORC_POSITION = 0x80
  };
  uint8_t countFlags() {
    // Count the number of flags to escape
    uint8_t n = 0;
    if (isFlag(type))
      n++;
    if (isFlag(value & 0x000000ff))
      n++;
    if (isFlag((value & 0x0000ff00) >> 8))
      n++;
    return n;
  }

  command_type type;
  uint16_t value;

  static bool isFlag(const uint8_t ch) {
    return ch == startflag || ch == endflag || ch == escapeflag;
  }
} command;

typedef void (*bus_receiver)(command cmd);

#endif // SERIALPACKETDEFINITION_H
