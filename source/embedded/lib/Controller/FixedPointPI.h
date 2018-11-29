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
        const uint16_t& Kp, const uint16_t& Ki, const int16_t& n,
        const uint16_t& delta_us = 0, const int16_t& Uinit = 0);
    ~FixedPointPI() = default;

    void setLimits(int16_t min, int16_t max);

    int16_t calculate(const int16_t& U) final;

    // Getters
    inline uint16_t getKp() const { return m_Kp; }
    inline uint16_t getKi() const { return m_Ki; }
    inline int16_t getN() const { return m_N; }

    // Setters
    inline void setKp(const uint16_t& Kp)
    {
        m_Kp       = Kp;
        m_scaledKp = m_Kp * m_deltaUs;
    }
    inline void setKi(const uint16_t& Ki)
    {
        m_Ki       = Ki;
        m_scaledKi = m_Ki * m_deltaUs;
    }
    inline void setDeltaUs(const uint16_t& deltaUs) final
    {
        m_deltaUs = deltaUs;
        setKp(m_Kp);
        setKi(m_Ki);
    }
    inline void setN(const int16_t& n)
    {
        m_N    = n;
        m_nMin = -(1 << (15 + m_N));
        m_nMax = (1 << (15 + m_N)) - 1;
    }

private:
    void satLimit();
    inline int32_t limit(int32_t x, int32_t min, int32_t max)
    {
        if (x < min)
            return min;
        else if (x > max)
            return max;
        else
            return x;
    }

private:
    uint16_t m_Kp       = 0;
    uint16_t m_Ki       = 0;
    uint16_t m_scaledKp = 0, m_scaledKi = 0;

    int32_t m_UMin32 = INT32_MIN;
    int32_t m_UMax32 = INT32_MAX;
    int16_t m_UMin16 = INT16_MIN;
    int16_t m_UMax16 = INT16_MAX;

    int16_t m_N    = 0;
    int32_t m_nMin = 0, m_nMax = 0;

    Integral m_Uintegral;
};

#endif // __FIXEDPOINTPI_H__
