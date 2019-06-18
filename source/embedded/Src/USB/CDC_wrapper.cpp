#include "CDC_wrapper.h"
#include "CDC.h"

void* CDC_create()
{
    return new CDC();
}

void CDC_destroy(void* thisCDC)
{
    delete static_cast<CDC*>(thisCDC);
}

void CDC_receive(void* thisCDC, uint8_t* buf, const uint16_t len)
{
    static_cast<CDC*>(thisCDC)->receive(buf, len);
}

void CDC_transmit(void* thisCDC, uint8_t* buf, const uint16_t len)
{
    static_cast<CDC*>(thisCDC)->transmit(buf, len);
}
