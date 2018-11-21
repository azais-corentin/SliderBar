#ifndef __USB_CDC_H__
#define __USB_CDC_H__

#include "usbd/usbd_cdc.h"
#include "usbd/usbd_def.h"

#include <DataInterface.h>

extern "C" {
// USB device core handle
extern USBD_HandleTypeDef hUsbDeviceFS;

// CDC Interface callback
extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS;
}

class USB_CDC : public DataInterface {
public:
    USB_CDC();
    ~USB_CDC();

    /** @brief USB Device initialization function.
      *  
      *  @retval None.
      */
    void initialise();

    /**
     * @brief Data received over USB OUT endpoint are sent over CDC interface
     * through this function.
     *
     * @note
     * This function will block any OUT packet reception on USB endpoint untill
     * exiting this function. If you exit this function before transfer is
     * complete on CDC interface (ie. using DMA controller) it will result in
     * receiving more data while previous ones are still not sent.
     *
     * @param buf: Buffer of data to be received
     * @param len: Number of data received (in bytes)
     * @retval Result of the operation: USBD_OK if all operations are OK else
     * USBD_FAIL
      */
    void receive(uint8_t* buf, uint16_t len) final;

    /** @brief Sends buf over USB IN endpoint.
     *  Sends buf over USB IN endpoint
     * 
     *  @param buf: Buffer of data to be sent
     *  @param len: Number of data to be sent (in bytes)
     * 
     *  @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
     */
    bool transmit(uint8_t* buf, uint16_t len) final;

    void setReceiver(DataInterface* _receiver);

private:
    DataInterface* receiver = nullptr;
};

extern USB_CDC* g_usb_cdc_ptr;

#endif // __USB_CDC_H__
