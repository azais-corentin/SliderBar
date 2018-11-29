#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

#include <DataInterface/DataInterface.h>

class SerialInterface : public QObject, public DataInInterface {
    Q_OBJECT

public:
    SerialInterface();

    void connect();
    void disconnect();

    inline void receive(uint8_t* buf, const uint16_t& len) final
    {
        if (receiver)
            receiver->receive(buf, len);
    }

    inline bool transmit(uint8_t* buf, const uint16_t& len) final
    {
        char datachar[len];
        memcpy(datachar, buf, len);
        return m_serialPort.write(datachar, len) != -1;
    }

signals:
    void connected();
    void disconnected();

private slots:
    void handleRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort m_serialPort;
};

#endif // SERIALINTERFACE_H
