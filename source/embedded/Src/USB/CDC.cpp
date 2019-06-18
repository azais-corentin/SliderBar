#include <USB/CDC.h>

#include <usbd_cdc_if.h>

#include <stm32f0xx_hal_pcd.h>

void CDC::receive(uint8_t* buf, const uint16_t& len) {
  if (m_receiver)
    m_receiver->receive(buf, len);
}

bool CDC::transmit(uint8_t* buf, const uint16_t& len) const {
  return CDC_Transmit_FS(buf, len) == USBD_OK;
}

CDC* CDC::self() {
  static CDC instance;
  return &instance;
}

void CDC::connect() {
  // Already connected
  if (m_connected)
    return;

  // Activate USB D+ pullup resistor, re-enumerates the device
  if (m_usbDevice)
    HAL_PCD_DevConnect(static_cast<PCD_HandleTypeDef*>(m_usbDevice->pData));

  m_connected = true;
}

void CDC::disconnect() {}
void CDC::setUSBDevice(USBD_HandleTypeDef* usbDevice) {
  m_usbDevice = usbDevice;
}
