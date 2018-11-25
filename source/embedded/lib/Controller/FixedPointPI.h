#ifndef __FIXEDPOINTPI_H__
#define __FIXEDPOINTPI_H__

#include "FixedPointController.hpp"

/**
 * @brief Implements a Fixed point PI controller
 * @note Implemented with the help of: https://www.embeddedrelated.com/showarticle/123.php
 * 
 * It need a Kp, Kd, a dt (in us) and Umin, Umax in order to be correct.
 * 
 */
class FixedPointPI : public FixedPointController {
    struct Integral {
        int32_t integral   = 0;
        int16_t saturation = 0;
    };

public:
    FixedPointPI(
        const int16_t& Kp, const int16_t& Ki,
        const uint16_t& delta_us = 0, const int16_t& Uinit = 0);
    ~FixedPointPI() = default;

    void setLimits(int16_t min, int16_t max);

    int16_t calculate(const int16_t& U) final;

    // Getters
    inline int16_t getKp() { return m_Kp; }
    inline int16_t getKi() { return m_Ki; }

    // Setters
    inline void setKp(int16_t Kp)
    {
        m_Kp       = Kp;
        m_scaledKp = m_Kp * m_deltaUs;
    }
    inline void setKi(int16_t Ki)
    {
        m_Ki       = Ki;
        m_scaledKi = m_Ki * m_deltaUs;
    }
    inline void setDeltaUs(const uint16_t& deltaUs) final
    {
        m_deltaUs = deltaUs;
        setKp(m_Kp);
        setKi(m_Ki);
    };

private:
    void satLimit();
    inline int32_t limit(int32_t x)
    {
    }

private:
    int16_t m_Kp = 0, m_Ki = 0;
    int16_t m_scaledKp = 0, m_scaledKi = 0;

    int32_t m_UMin32 = INT32_MIN;
    int32_t m_UMax32 = INT32_MAX;

    Integral m_Uintegral;
};

#endif // __FIXEDPOINTPI_H__
