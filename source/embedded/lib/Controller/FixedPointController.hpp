#ifndef __FIXEDPOINTCONTROLLER_HPP__
#define __FIXEDPOINTCONTROLLER_HPP__

#include <cstdint>

class FixedPointController {
public:
    FixedPointController(const uint16_t& deltaUs = 0, const int16_t& Uinit = 0)
        : m_deltaUs(deltaUs)
        , m_U(Uinit)
    {
    }
    virtual ~FixedPointController() = default;

    virtual int16_t calculate(const int16_t& U) = 0;

    // Getters
    inline int16_t getTarget() { return m_Uc; }
    inline uint16_t getDeltaUs() { return m_deltaUs; }

    // Setters
    inline void setTarget(const uint16_t& target)
    {
        m_Uc = target;
    }
    inline virtual void setDeltaUs(const uint16_t& deltaUs) = 0;

protected:
    int16_t m_U        = 0;
    int16_t m_Uc       = 0;
    uint16_t m_deltaUs = 0;
};

#endif // __FIXEDPOINTCONTROLLER_HPP__
