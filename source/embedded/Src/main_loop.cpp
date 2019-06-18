//
// Created by Nelieru on 06/06/2019.
//

#include <cstring>

#include "stm32f0xx_hal.h"

#include "main_loop.h"
#include <SliderBar/sliderbar.h>
#include <USB/CDC.h>

static usb::CDC cdc;
static SliderBar sliderbar;

void main_loop()
{
    cdc.initialize();

    cdc.setReceiver(&sliderbar);
    sliderbar.setTransmitter(&cdc);

    while (true) {
        sliderbar.decode();
    }
}
