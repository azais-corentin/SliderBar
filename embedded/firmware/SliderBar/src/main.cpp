#include <mbed.h>

Serial pc(USBTX, USBRX, 115200); // tx, rx

int main()
{

    while (1)
    {
        pc.printf("");
    }
}