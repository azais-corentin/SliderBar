#include "SliderBarSim.h"

SliderBarSim::SliderBarSim()
{
}

SliderBarSim::~SliderBarSim()
{
}

float SliderBarSim::update(float dt, float motorSpeed)
{
    float alpha = dt / (dt + tau);
    position = position * alpha + K * (1 - alpha) * dt * motorSpeed;
}