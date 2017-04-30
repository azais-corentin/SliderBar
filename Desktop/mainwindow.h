#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPortInfo>

#include "settingsdialog.h"
#include "serialpacketdefinition.h"

namespace Ui {
class MainWindow;
}

class SerialProtocol;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writePacket(command& packet);
    void receivePacket(command& packet);

    int showConfiguration();

private:
    void loadSettings();
    void toggleAutoconnect();

    void initConnections();

    void showStatusMessage(const QString& message);

    Ui::MainWindow* ui;
    QLabel* m_pStatus;

    SettingsDialog* m_pSettingsDialog;
    QSettings m_settings;

    SerialProtocol* m_pSerial;
};

#endif // MAINWINDOW_H
