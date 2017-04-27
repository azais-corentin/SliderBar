#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSettings>
#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>

#include "serialsettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray& data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);

    void showConfiguration();

private:
    void saveSerialConfig();
    void loadSerialConfig();

    void initActionsConnections();

    void showStatusMessage(const QString& message);

    Ui::MainWindow* ui;
    QLabel* m_pStatus;
    QSettings m_Settings;
    bool m_serialSettingsLoaded;

    SerialSettings* m_pSerialSettings;
    QSerialPort* m_pSerial;
};

#endif // MAINWINDOW_H
