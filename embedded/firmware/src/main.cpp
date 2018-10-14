#include <mbed.h>

#include <SliderBarSim.h>

Serial pc(USBTX, USBRX, 921600); // tx, rx

float motorspeed;
float lasttime;
float position;

int main()
{
    SliderBarSim sb;
    Timer t;
    t.start();

    motorspeed = sin(t.read());
    lasttime = t.read();

    while (1)
    {
        motorspeed = sin(t.read());
        if (motorspeed > 0)
            motorspeed = 1.f;
        else
            motorspeed = -1.f;

        position = sb.update(t.read() - lasttime, motorspeed);
        pc.printf("%f, %f\n\r", motorspeed, position);
        lasttime = t.read();
    }
}