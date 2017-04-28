#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialprotocol.h"

#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pStatus = new QLabel;
    ui->statusBar->addWidget(m_pStatus);
    ui->actionDisconnect->setEnabled(false);

    m_pSerial = new SerialProtocol;

    initConnections();

    m_pSettingsDialog = new SettingsDialog(this);
    loadSettings();

    if (m_settings.value("autoConnect", false).toBool())
    {
        openSerialPort();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort()
{
    if (m_pSerial->openSerialPort())
    {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        return;
    }
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::closeSerialPort()
{
    m_pSerial->closeSerialPort();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::writeData(const QByteArray& data)
{
    m_pSerial->writeData(data);
}

void MainWindow::receiveData(const QByteArray& data)
{

}

int MainWindow::showConfiguration()
{
    int execute = m_pSettingsDialog->execute();
    loadSettings();
    return execute;
}

void MainWindow::loadSettings()
{
    // Load serial port configuration
    m_pSerial->loadSettings();
    ui->actionAuto_connect->setChecked(m_settings.value("serial/autoconnect", false).toBool());
}

void MainWindow::toggleAutoconnect()
{
    m_settings.setValue("serial/autoconnect", ui->actionAuto_connect->isChecked());
}

void MainWindow::initConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::showConfiguration);
    connect(ui->actionAuto_connect, &QAction::triggered, this, &MainWindow::toggleAutoconnect);

    connect(m_pSerial, &SerialProtocol::statusMessage, this, &MainWindow::showStatusMessage);
    connect(m_pSerial, &SerialProtocol::packetReady, this, &MainWindow::receiveData);
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_pStatus->setText(message);
}
