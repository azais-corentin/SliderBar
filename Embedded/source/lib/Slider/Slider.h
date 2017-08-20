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
    uint16_t getPosition();

    double getEstPosition() { return m_xk_1; }
    double getEstVelocity() { return m_vk_1; }

    void setSpeed(uint16_t val);
    int getSpeed() { return m_tSpeed; }

    void setMode(int mode) { m_pPID->SetMode(mode); }
    void setP(double P) {
        m_pPID->SetTunings(P, m_pPID->GetKi(), m_pPID->GetKd());
    }
    void setI(double I) {
        m_pPID->SetTunings(m_pPID->GetKp(), I, m_pPID->GetKd());
    }
    void setD(double D) {
        m_pPID->SetTunings(m_pPID->GetKp(), m_pPID->GetKi(), D);
    }

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

    // Tracking filter part
    unsigned long old_time;
    // double m_a = 0.18, m_b = 0.00178459, m_y = 0.00088466;
    double m_alpha = 0.001;
    double m_xk_1 = 0., m_vk_1 = 0.;
};

#endif // SLIDER_H
