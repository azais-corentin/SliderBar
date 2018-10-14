#include "SliderBarSim.h"

SliderBarSim::SliderBarSim()
{
}

SliderBarSim::~SliderBarSim()
{
}

float SliderBarSim::update(float dt, float motorSpeed)
{
    float alpha = dt / tau;
    position = alpha * (position + dt * K * motorSpeed) + (1.f - alpha) * position;
    //position = position + dt * K * motorSpeed;
    if (position < 0.f)
        position = 0.f;
    else if (position > 1.0f)
        position = 1.0f;
    return position;
}