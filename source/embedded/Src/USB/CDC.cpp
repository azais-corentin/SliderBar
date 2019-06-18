#include <USB/CDC.h>

#include <usbd_cdc_if.h>

void CDC::receive(uint8_t* buf, const uint16_t& len)
{
    if (m_receiver)
        m_receiver->receive(buf, len);
}

bool CDC::transmit(uint8_t* buf, const uint16_t& len) const
{
    return CDC_Transmit_FS(buf, len) == USBD_OK;
}

CDC* CDC::self()
{
    static CDC instance;
    return &instance;
}
void CDC::connect()
{
    if (!m_connected) {
        /// @todo Implement USB connect
        HAL_PCD_DevConnect();
        m_connected = true;
    }
}

void CDC::disconnect() {}
