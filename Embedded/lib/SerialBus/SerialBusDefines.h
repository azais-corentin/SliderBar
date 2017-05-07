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
    FORS_POSITION = 0x01, // uint16_t position, v*655.36mm
    FORS_SPEED,           // uint16_t speed, m/s
    FORS_VIBRATE,

    FORS_RESIST_AT,
    FORS_RESIST_CLEAR,

    /// FOR COMPUTER
    // From Slider to Computer
    FORC_POSITION = 0x80 // uint16_t position, v*655.36mm
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