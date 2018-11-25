#include "FixedPointPI.h"

FixedPointPI::FixedPointPI(
    const int16_t& Kp, const int16_t& Ki,
    const uint16_t& delta_us = 0, const int16_t& Uinit = 0)
    : m_Kp(Kp)
    , m_Ki(Ki)
{
    setDeltaUs(delta_us);
    m_U = Uinit;
}

void FixedPointPI::setLimits(int16_t min, int16_t max)
{
    m_UMin32 = ((int32_t)min) << 16;
    m_UMax32 = ((int32_t)max) << 16;
}

int16_t FixedPointPI::calculate(const int16_t& U)
{
    m_U = U;

    int16_t e = m_Uc - m_U;

    // Equivalent: if (!(sat < 0 && e < 0) && !(sat > 0 && e > 0))
    // Do nothing if there is saturation and error is in the same direction
    if (m_Uintegral.saturation * e <= 0)
        m_Uintegral.integral += m_scaledKi * e;

    // Updates the saturation & integral fields
    satLimit();

    //int32_t p_term = limit(static_cast<int32_t>(m_scaledKp)*e, )
}

void FixedPointPI::satLimit()
{
    if (m_Uintegral.integral < m_UMin32) {
        m_Uintegral.integral   = m_UMin32;
        m_Uintegral.saturation = -1;
    } else if (m_Uintegral.integral > m_UMax32) {
        m_Uintegral.integral   = m_UMax32;
        m_Uintegral.saturation = 1;
    } else {
        m_Uintegral.saturation = 0;
    }
}