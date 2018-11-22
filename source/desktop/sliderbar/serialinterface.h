#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

#include <DataInterface/DataInterface.h>

class SerialInterface : public DataInInterface {
public:
    SerialInterface();

    void receive(uint8_t* buf, uint16_t len) final;
    bool transmit(uint8_t* buf, uint16_t len) final;

private:
    void update();

private:
    QSerialPort m_port;
    QTimer* m_timer = nullptr;
};

#endif // SERIALINTERFACE_H
