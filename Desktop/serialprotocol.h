#ifndef SERIALPROTOCOL_H
#define SERIALPROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSettings>
#include <QLinkedList>

class SerialProtocol : public QObject
{
    Q_OBJECT

public:
    SerialProtocol();
    ~SerialProtocol();

    void initConnections();
    void loadSettings();

    bool openSerialPort();
    void closeSerialPort();

    void writeData(const QByteArray& data); //encodes data then sends it
    void writeData(const QVector<uchar>& data);

    const uchar* convertByteArray(const QByteArray& data);

    QSerialPort* serial;

signals:
    void packetReady(const QByteArray& data);
    void statusMessage(QString message);
    void disconnected();

private slots:
    void readData(); //read data and emits packetReady() once a packet is totally received
    void handleError(QSerialPort::SerialPortError error);

private:
    uchar startflag, endflag, escapeflag, xorflag;

    uint32_t totalPackets, failedPackets;

    QSettings m_settings;
    QByteArray m_buffer;
};

#endif // SERIALPROTOCOL_H
