#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSettings>
#include <QtSerialPort/QSerialPortInfo>

#include "settingsdialog.h"
#include "serialpacketdefinition.h"

#include "systemkeyboardhook.h"

namespace Ui {
class MainWindow;
}

class SerialProtocol;
class QLabel;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void disableConnect();
    void enableConnect();
    void receivePacket(const command& packet);
    void clearStatusMessage();

    int showConfiguration();

    void on_eProgress_valueChanged(int value);
    void handleKeyPressed(DWORD key);

    void on_bAddResist_clicked();
    void on_bClearResists_clicked();
    void on_bDisablePID_clicked();
    void on_bEnablePID_clicked();
    void on_bUpdatePID_clicked();

private:
    void loadSettings();
    void toggleAutoconnect();

    void initConnections();

    void showStatusMessage(const QString& message);

    Ui::MainWindow* ui;
    QLabel* m_pStatus;
    QTimer* m_pTimerStatusMessage;

    SystemKeyboardHook* m_pSystemKeyboardHook;

    SettingsDialog* m_pSettingsDialog;
    QSettings m_settings;

    SerialProtocol* m_pSerial;
    uint16_t m_sliderPos = 0;
    uint32_t m_count = 0;
};

#endif // MAINWINDOW_H
