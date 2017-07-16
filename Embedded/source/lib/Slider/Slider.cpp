#include "Slider.h"

#include "SerialBusDefines.h"

Slider::Slider() {
    m_pMotor = new Motor(6, 3, 5, -1, 7);
    // 1.2, 2, 0.012
    m_pPID =
        new PID(&m_cPosition, &m_tSpeed, &m_tPosition, 1.5, 50, 0.010, DIRECT);
    m_pPID->SetOutputLimits(-255, 255);
    m_pPID->SetSampleTime(10);
    m_pPID->SetMode(AUTOMATIC);

    for (uint8_t i = 0; i < MAX_RESIST; i++)
        m_pResist_at[i] = -1;

    m_tPosition = 512; // middle
    m_cPosition = analogRead(A4);
}

Slider::~Slider() {}

void Slider::update() {
    // Read current slider position
    m_cPosition = analogRead(A4);
    double dt = 0.001;
    m_xk_1 += (m_vk_1 * dt + m_acc * dt * dt / 2.);

    double rk = ((double)m_cPosition) - m_xk_1;

    m_xk_1 += m_a * rk;              // estimated position
    m_vk_1 += dt * m_acc + m_b * rk; // estimated velocity
    m_acc += m_g * rk;

    m_cPosition = m_xk_1;

    // Continue if PID is active
    if (m_pPID->GetMode() == MANUAL) {
        // Brake if we're close to a resist_at point
        if (m_resistIndex > 0)
            if (approxBinarySearch(m_pResist_at, m_resistIndex, m_cPosition,
                                   20) >= 0) {
                m_pMotor->drive(30);
                /*delay(10);
                m_pMotor->drive(-50);
                delay(10);*/
                m_pMotor->brake();
            }

        return;
    }

    if (abs(m_tPosition - m_cPosition) < 4)
        m_pPID->resetI();

    // Prevent I overshoot if far from position
    if (abs(m_tPosition - m_cPosition) < 50)
        m_pPID->setIenabled(true);
    else
        m_pPID->setIenabled(false);

    // Compute PID values
    m_pPID->Compute();

    // Prevent slow speeds (beeping of the motor)
    if (abs(m_tSpeed) < 70) {
        m_pMotor->brake();
        m_pPID->setIenabled(false);
    } else
        m_pMotor->drive(static_cast<int>(m_tSpeed));
}

void Slider::setPosition(uint16_t val) { m_tPosition = val; }

void Slider::setSpeed(uint16_t val) { m_tSpeed = val - 255; }

void Slider::appendResist(uint16_t resist_at) {
    if (m_resistIndex >= MAX_RESIST)
        return;

    m_pResist_at[m_resistIndex++] = resist_at;
}

int Slider::approxBinarySearch(uint16_t *arr, uint8_t r, uint16_t x,
                               uint8_t prec) {
    if (r == 0)
        return -1;
    if (uint16_t(x - prec) > x) {
        if (arr[0] <= x)
            return 0;
        else
            return -1;
    }

    uint8_t l = 0;
    while (l <= r) {
        uint8_t m = l + (r - l) / 2;

        // Check if x is present at mid
        if (arr[m] <= x + prec && arr[m] >= x - prec)
            return m;

        // If x greater, ignore left half
        if (arr[m] < x)
            l = m + 1;

        // If x is smaller, ignore right half
        else
            r = m - 1;
    }

    // if we reach here, then element was not present
    return -1;
}

void Slider::clearResists() {
    m_resistIndex = 0;
    for (uint8_t i = 0; i < MAX_RESIST; i++)
        m_pResist_at[i] = -1;
}

uint16_t Slider::getPosition() { return m_cPosition; }
