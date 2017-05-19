#include "Slider.h"

Slider::Slider() {
    m_pMotor = new Motor(6, 3, 5, -1, 7);
    // 1.2, 2, 0.012
    m_pPID =
        new PID(&m_cPosition, &m_tSpeed, &m_tPosition, 1.2, 3, 0.010, DIRECT);
    m_pPID->SetOutputLimits(-255, 255);
    m_pPID->SetSampleTime(10);
    m_pPID->SetMode(AUTOMATIC);

    m_tPosition = 512; // middle
    m_cPosition = analogRead(A4);
}

Slider::~Slider() {}

void Slider::update() {
    m_cPosition = analogRead(A4);
    m_pPID->Compute();

    if (abs(m_tSpeed) < 40)
        m_pMotor->brake();
    else
        m_pMotor->drive(static_cast<int>(m_tSpeed));
}

void Slider::setPosition(uint16_t val) { m_tPosition = val; }

void Slider::setSpeed(uint16_t val) {
    m_tSpeed =
        255. * static_cast<double>(static_cast<int>(val) - 32768) / 32768.;
}
