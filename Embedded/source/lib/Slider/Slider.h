#ifndef SLIDER_H
#define SLIDER_H

#include "Arduino.h"

#include "PID.h"
#include "TB6612FNG.h"

#define MAX_RESIST 10

class Slider {
  public:
    Slider();
    virtual ~Slider();

    void update();

    void setPosition(uint16_t val);
    uint16_t getPosition() { return m_cPosition; }
    void setSpeed(uint16_t val);
    int getSpeed() { return m_tSpeed; }

    void setMode(int mode) { m_pPID->SetMode(mode); }

    void appendResist(uint16_t resist_at);
    void clearResists();

  private:
    int approxBinarySearch(uint16_t arr[], uint8_t r, uint16_t x, uint8_t prec);

    uint16_t m_pResist_at[MAX_RESIST] = {0};
    uint8_t m_resistIndex = 0;
    Motor *m_pMotor;
    PID *m_pPID;
    double m_cPosition = 0, m_tPosition = 0;
    double m_tSpeed = 0;
};

#endif // SLIDER_H
