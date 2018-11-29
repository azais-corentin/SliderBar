#ifndef __FIXEDPOINTCONTROLLER_HPP__
#define __FIXEDPOINTCONTROLLER_HPP__

#include <cstdint>

class FixedPointController {
public:
    FixedPointController(const int16_t& Uinit = 0)
        : m_U(Uinit)
    {
    }

    FixedPointController(const int16_t& Uinit, const uint16_t& deltaUs)
        : m_deltaUs(deltaUs)
        , m_U(Uinit)
    {
    }
    virtual ~FixedPointController() = default;

    virtual int16_t calculate(const int16_t& U) = 0;

    // Getters
    inline virtual int16_t getTarget() const { return m_Uc; }
    inline virtual uint16_t getDeltaUs() const { return m_deltaUs; }

    // Setters
    inline virtual void setTarget(const uint16_t& target)
    {
        m_Uc = target;
    }
    inline virtual void setDeltaUs(const uint16_t& deltaUs) = 0;

protected:
    uint16_t m_deltaUs = 0;
    int16_t m_U        = 0;
    int16_t m_Uc       = 0;
    int16_t m_Uout     = 0;
};

#endif // __FIXEDPOINTCONTROLLER_HPP__
