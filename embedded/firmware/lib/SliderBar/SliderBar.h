#ifndef __SLIDERBAR_H__
#define __SLIDERBAR_H__

#include <cstdint>

#include "usbd/usbd_def.h"

#include "Buffer.hpp"

class SliderBar {
public:
    SliderBar();
    ~SliderBar();

    /**
     *  @brief Runs the main loop of the SliderBar
     *  It decodes the messages sent through USB, and moves the motor
     *  accordingly.
     *  
     *  @retval None
     */
    void run();

    void receive(uint8_t* buf, uint32_t* len);

private:
    Buffer<2 * USB_FS_MAX_PACKET_SIZE> m_buffer;

    void decode();
};

extern SliderBar* g_sliderbar_ptr;

#endif // __SLIDERBAR_H__
