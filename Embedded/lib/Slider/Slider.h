#ifndef SLIDER_H
#define SLIDER_H

#include "Arduino.h"

#include "PID.h"
#include "TB6612FNG.h"

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

  private:
    float polynomial[2] = {0.0968879, 0.42659893};
    Motor *m_pMotor;
    PID *m_pPID;
    double m_cPosition = 0, m_tPosition = 0;
    double m_tSpeed = 0;
};

#endif // SLIDER_H
