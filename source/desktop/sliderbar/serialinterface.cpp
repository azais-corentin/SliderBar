#include "serialinterface.h"

#include <QDebug>

SerialInterface::SerialInterface()
{
    auto ports = QSerialPortInfo::availablePorts();
    for (const auto& port : ports) {
        if (port.productIdentifier() == 0x5740)
            m_port.setPort(port);
    }

    m_port.open(QIODevice::ReadWrite);

    m_timer = new QTimer();
    QObject::connect(m_timer, &QTimer::timeout, [this] {
        update();
    });

    m_timer->start(10);
}

void SerialInterface::receive(uint8_t* buf, uint16_t len)
{
    if (receiver)
        receiver->receive(buf, len);
}

bool SerialInterface::transmit(uint8_t* buf, uint16_t len)
{
    char datachar[len];
    memcpy(datachar, buf, len);
    return m_port.write(datachar, len) != -1;
}

void SerialInterface::update()
{
    if (m_port.bytesAvailable() > 0) {
        auto data = m_port.readAll();
        uint8_t dataui8[data.size()];
        memcpy(dataui8, data.data(), data.size());
        receive(dataui8, data.size());
    }
}
