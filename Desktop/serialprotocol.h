#ifndef SERIALPROTOCOL_H
#define SERIALPROTOCOL_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QSettings>

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

    QSerialPort* serial;

signals:
    void packetReady(const QByteArray& data);
    void statusMessage(QString message);

private slots:
    void readData(); //read data and emits packetReady() once a packet is totally received
    void handleError(QSerialPort::SerialPortError error);

private:
    int startflag, endflag, escapeflag, xorflag;

    QSettings m_settings;
    QByteArray m_buffer;
};

#endif // SERIALPROTOCOL_H
