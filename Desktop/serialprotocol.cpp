#include "serialprotocol.h"

#include <QString>
#include <QMessageBox>
#include <QDebug>

SerialProtocol::SerialProtocol()
{
    serial = new QSerialPort;

    loadSettings();

    m_buffer.clear();

    initConnections();
}

SerialProtocol::~SerialProtocol()
{
    delete serial;
}

void SerialProtocol::initConnections()
{
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
        this, &SerialProtocol::handleError);

    connect(serial, &QSerialPort::readyRead, this, &SerialProtocol::readData);
}

void SerialProtocol::loadSettings()
{
    startflag = m_settings.value("serial/protocol/startflag", 0x12).toInt();
    endflag = m_settings.value("serial/protocol/endflag", 0x13).toInt();
    escapeflag = m_settings.value("serial/protocol/escapeflag", 0x7D).toInt();
    xorflag = m_settings.value("serial/protocol/xorflag", 0x20).toInt();

    serial->setPortName(m_settings.value("serial/portname", "COM1").toString());
    serial->setBaudRate(static_cast<QSerialPort::BaudRate>(
            m_settings.value("serial/baudrate", QSerialPort::Baud115200).toInt()),
        QSerialPort::AllDirections);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(
            m_settings.value("serial/databits", QSerialPort::Data8).toInt()));
    serial->setParity(static_cast<QSerialPort::Parity>(
            m_settings.value("serial/parity", QSerialPort::NoParity).toInt()));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(
            m_settings.value("serial/stopbits", QSerialPort::OneStop).toInt()));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(
            m_settings.value("serial/flowcontrol", QSerialPort::HardwareControl).toInt()));
}

bool SerialProtocol::openSerialPort()
{
    loadSettings();
    if (serial->isOpen())
    {
        emit statusMessage(QStringLiteral("Serial port: Already connected!"));
        return true;
    }
    if (serial->open(QIODevice::ReadWrite))
    {
        emit statusMessage(QStringLiteral("Serial port: Connected to %1").arg(serial->portName()));
        return true;
    }
    else
    {
        emit statusMessage(QStringLiteral("Serial port: Error %1").arg(serial->errorString()));
    }
    return false;
}

void SerialProtocol::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    emit statusMessage(QStringLiteral("Disconnected!"));
}

void SerialProtocol::writeData(const QByteArray& data)
{
    /// Packet structure:
    /// http://eli.thegreenplace.net/2009/08/12/framing-in-serial-communications/
    /// [0]         startflag
    /// [1,2]       x (length)
    /// [3,..,x-1]  data
    /// [x]         endflag
    QByteArray packet;
    int length = 1 + 2 + data.size() + data.count(startflag) + data.count(endflag) +
        data.count(escapeflag) + 1;
    packet.resize(length);
    int i = 0, di = 0;

    // Add startflag
    packet[i++] = char(startflag);

    // Add length
    //packet.replace(i, 2, QByteArray::number(qint16(length), 16));

    // Add data
    while (di < data.size())
    {
        if (data.at(di) == startflag || data.at(di) == endflag || data.at(di) == escapeflag)
        {
            packet[i++] = char(escapeflag);
            packet[i++] = data.at(di) ^ char(xorflag);
        }
        else
            packet[i++] = data.at(di);
        di++;
    }

    // Add endflag
    packet[i] = char(endflag);
    foreach (char ch, packet)
    {
        qDebug() << QString::number(ch, 16);
    }
}

void SerialProtocol::readData()
{
    QByteArray data = serial->readAll();
    qDebug() << data;
}

void SerialProtocol::handleError(QSerialPort::SerialPortError error)
{
    emit statusMessage(QStringLiteral("Serial port: Error %1").arg(serial->errorString()));
}
