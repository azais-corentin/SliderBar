#include "main_loop.h"

#include <SliderBar/sliderbar.h>
#include <USB/CDC.h>

// Initializes global instances
void* cdc       = CDC::self();
void* sliderbar = SliderBar::self();

void main_loop()
{
    auto&& cdcInstance       = static_cast<CDC*>(cdc);
    auto&& sliderbarInstance = static_cast<SliderBar*>(sliderbar);

    cdcInstance->initialize();

    cdcInstance->setReceiver(sliderbarInstance);
    sliderbarInstance->setTransmitter(cdcInstance);

    while (true) {
        sliderbarInstance->decode();
    }
}
