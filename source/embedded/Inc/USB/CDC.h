#ifndef EMBEDDED_CDC_H
#define EMBEDDED_CDC_H

#include <cstdint>

#include <DataInterface/DataInterface.h>

#include <usbd_cdc.h>

extern "C" {
// USB device core handle
extern USBD_HandleTypeDef hUsbDeviceFS;

// CDC Interface callback
extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS;
}

namespace usb {

class CDC;

extern CDC* g_cdc_ptr;

class CDC : public DataInInterface {
public:
    CDC();

    /**
     * @brief Initializes the USB Device functions
     */
    void initialize();

    void connect() final {};
    void disconnect() final {};

    /**
     * @brief Data received over USB OUT endpoint are sent over CDC interface
     * through this function.
     *
     * @note This function will block any OUT packet reception on USB endpoint untill
     * exiting this function. If you exit this function before transfer is
     * complete on CDC interface (ie. using DMA controller) it will result in
     * receiving more data while previous ones are still not sent.
     *
     * @param buf Buffer of data to be received
     * @param len Number of data received (in bytes)
     */
    void receive(uint8_t* buf, const uint16_t& len) final;

    /** @brief Sends buf over USB IN endpoint.
     *  Sends buf over USB IN endpoint
     *
     *  @param buf: Buffer of data to be sent
     *  @param len: Number of data to be sent (in bytes)
     *
     *  @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
     */

    /**
     * @brief Sends buf over the USB IN endpoint.
     * @param buf Buffer of data to be sent
     * @param len Number of data to be sent (in bytes)
     * @return USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
     */
    bool transmit(uint8_t* buf, const uint16_t& len) const final;
};

} // namespace usb

#endif //EMBEDDED_CDC_H
