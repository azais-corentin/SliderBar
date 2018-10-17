#ifndef __USB_CDC_H__
#define __USB_CDC_H__

#include "usbd/usbd_cdc.h"
#include "usbd/usbd_def.h"

extern "C" {
// USB device core handle
extern USBD_HandleTypeDef hUsbDeviceFS;

// CDC Interface callback
extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS;
}

class USB_CDC {
public:
    USB_CDC();
    ~USB_CDC();

    /** @brief USB Device initialization function.
      *  
      *  @retval None.
      */
    void initialise();

    /** @brief Sends buf over USB IN endpoint.
     *  Sends buf over USB IN endpoint
     * 
     *  @param buf: Buffer of data to be sent
     *  @param len: Number of data to be sent (in bytes)
     * 
     *  @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
     */
    uint8_t transmit(uint8_t* buf, uint16_t len);

    uint8_t receive(uint8_t* buf, uint32_t* len);
};

extern USB_CDC cdc;

#endif // __USB_CDC_H__
