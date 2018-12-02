#include "serialinterface.h"

#include <QDebug>

namespace SliderBar {

SerialInterface::SerialInterface()
{
    QObject::connect(&m_serialPort, &QSerialPort::readyRead,
                     this, &SerialInterface::handleRead);
    QObject::connect(&m_serialPort, &QSerialPort::errorOccurred,
                     this, &SerialInterface::handleError);
}

void SerialInterface::connect()
{
    if (m_serialPort.isOpen())
        return;

    auto ports = QSerialPortInfo::availablePorts();
    for (const auto& port : ports) {
        if (port.productIdentifier() == 0x5740)
            m_serialPort.setPort(port);
    }

    if (m_serialPort.open(QIODevice::ReadWrite)) {
        emit connected();
        qDebug() << "Emit SerialInterface::connected";
    } else
        disconnect();
}

void SerialInterface::disconnect()
{
    m_serialPort.close();
    emit disconnected();
}

void SerialInterface::handleRead()
{
    auto data = m_serialPort.readAll();
    uint8_t dataui8[data.size()];
    memcpy(dataui8, data.data(), data.size());
    receive(dataui8, data.size());
}

void SerialInterface::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ReadError) {
        disconnect();
    }
}

} // namespace SliderBar
