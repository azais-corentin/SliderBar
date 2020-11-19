#ifndef EMBEDDED_CDC_H
#define EMBEDDED_CDC_H

#include <cstdint>

#include <DataInterface/DataInterface.h>
#include "usbd_def.h"

class CDC;

class CDC : public DataInInterface {
 public:
  static CDC* self();
  CDC() = default;
  ~CDC() = default;

  void setUSBDevice(USBD_HandleTypeDef* usbDevice);

  /**
   * @brief Connects to the USB host.
   * @note Calling this before calling disconnect() does nothing because USB
   * is connected by default.
   */
  void connect() final;

  /**
   * @brief Disconnects from the USB host.
   */
  void disconnect() final;

  /**
   * @brief Data received over USB OUT endpoint are sent over CDC interface
   * through this function.
   *
   * @note This function will block any OUT packet reception on USB endpoint
   * untill exiting this function. If you exit this function before transfer
   * is complete on CDC interface (ie. using DMA controller) it will result in
   * receiving more data while previous ones are still not sent.
   *
   * @param buf Buffer of data to be received
   * @param len Number of data received (in bytes)
   */
  void receive(uint8_t* buf, const uint16_t& len) final;

  /**
   * @brief Sends buf over the USB IN endpoint.
   * @param buf Buffer of data to be sent
   * @param len Number of data to be sent (in bytes)
   * @return true if all operations are OK
   * @return false if there was a problem
   */
  bool transmit(uint8_t* buf, const uint16_t& len) const final;

 private:
  bool m_connected = true;
  USBD_HandleTypeDef* m_usbDevice = nullptr;
};

#endif  // EMBEDDED_CDC_H
