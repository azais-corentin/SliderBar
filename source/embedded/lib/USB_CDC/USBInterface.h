#ifndef __USBINTERFACE_H__
#define __USBINTERFACE_H__

#include <cstdint>

/**
 * @brief Defines the USB Interface.
 * A class should inherit the USBInterface and implement the 
 * void receive(uint8_t* buf, uint8_t len)
 * method in order to receive data from the data layer.
 * 
 */
class USBInterface {
public:
    //virtual ~USBInterface();

    /**
     * @brief The receive method.
     * The receive method should be reimplemented by child classes.
     * It will be called everytime new data is available for the data layer.
     * 
     * @param buf The data bytes.
     * @param len The length of the data (in bytes).
     */
    virtual void receive(uint8_t* buf, uint8_t len) = 0;
};

#endif // __USBINTERFACE_H__
