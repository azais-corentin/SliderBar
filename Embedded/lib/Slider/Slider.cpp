#include "Slider.h"

Slider::Slider() {
    m_pMotor = new Motor(6, 3, 5, -1, 7);
    m_pPID =
        new PID(&m_cPosition, &m_tSpeed, &m_tPosition, 3, 10, 0.03, DIRECT);
    m_pPID->SetOutputLimits(-255, 255);
    m_pPID->SetSampleTime(10);
    m_pPID->SetMode(AUTOMATIC);

    m_tPosition = 512; // middle
}

Slider::~Slider() {}

void Slider::update() {
    m_cPosition = analogRead(A4);
    if (abs(m_tPosition - m_cPosition) <= 10) {
        m_tSpeed = 0;
        m_pMotor->drive(0);
    } else {
        m_pPID->Compute();
        m_pMotor->drive(static_cast<int>(m_tSpeed));
    }
}

void Slider::setPosition(uint16_t val) { m_tPosition = val; }

void Slider::setSpeed(uint16_t val) {
    m_tSpeed =
        255. * static_cast<double>(static_cast<int>(val) - 32768) / 32768.;
}
