#ifndef __SLIDERBAR_H__
#define __SLIDERBAR_H__

#include <cstdint>

//#include "../USB_CDC/USBInterface.h"
#include <USBInterface.h>
#include <protocol/protodef.h>
#include <usbd/usbd_def.h>

//const uint8_t MAX_PACKET_SIZE = USB_FS_MAX_PACKET_SIZE;

template <uint8_t N>
class Buffer;

/**
 * @brief The SliderBar class.
 * Serves as the main loop of the SliderBar system.
 * This receives commands through USB, and execute actions accordingly.
 * 
 */
class SliderBar : public USBInterface {
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

    /**
     * @brief 
     * Called by the data layer (ie: USB, serial, ...) when there is new data
     * available. This should normally contain startflag, packet data 
     * (type, value, crc) and endflag.
     *
     * @param buf Pointer to the data.
     * @param len Length of the data.
     */
    void receive(uint8_t* buf, uint8_t len) override;

private:
    Buffer<protocol::MAX_PACKET_SIZE>* m_buffer = nullptr;
    bool newData = false;

    void decode();
};

extern SliderBar* g_sliderbar_ptr;

#endif // __SLIDERBAR_H__
