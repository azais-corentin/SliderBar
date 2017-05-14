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
    ui->progressBar->setRange(0, 65536);
    enableConnect();

    m_pSerial = new SerialProtocol;
    m_pSettingsDialog = new SettingsDialog(this);
    initConnections();
    loadSettings();

    if (m_settings.value("autoConnect", false).toBool())
        openSerialPort();

    ipacket = 0;
    averageTime = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort()
{
    m_pSerial->openSerialPort();
}

void MainWindow::closeSerialPort()
{
    m_pSerial->closeSerialPort();
}

void MainWindow::disableConnect()
{
    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
}

void MainWindow::enableConnect()
{
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
}

void MainWindow::writePacket(command& packet)
{
    m_pSerial->writePacket(packet);
}

void MainWindow::receivePacket(const command& packet)
{
    ipacket++;
    if (ipacket > 50)
    {
        averageTime = 0.2f * float(m_timerLatency.nsecsElapsed()) / 1000000.f + 0.8f * averageTime;
        qDebug() << averageTime / (2.f * 50.f);
        ipacket = 0;
        on_bSendPacket_clicked();
    }
    else
    {
        command packet;
        packet.type = command::FORS_POSITION;
        packet.value = static_cast<uint16_t>(25 * 655.36);
        m_pSerial->writePacket(packet);
    }

    switch (packet.type)
    {
        case command::FORC_POSITION:
            ui->progressBar->setValue(packet.value);
            //qDebug() << "Slider position:" << packet.value / 655.36;
            break;
        default:
            qDebug() << "Error: Packet not meant for computer!";
            break;
    }
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

    connect(m_pSerial, &SerialProtocol::serialConnected, this, &MainWindow::disableConnect);
    connect(m_pSerial, &SerialProtocol::serialDisconnected, this, &MainWindow::enableConnect);
    connect(m_pSerial, &SerialProtocol::statusMessage, this, &MainWindow::showStatusMessage);
    connect(m_pSerial, &SerialProtocol::packetReady, this, &MainWindow::receivePacket);
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_pStatus->setText(message);
}

void MainWindow::on_bSendPacket_clicked()
{
    m_timerLatency.start();
    command packet;
    packet.type = command::FORS_POSITION;
    packet.value = static_cast<uint16_t>(25 * 655.36);
    m_pSerial->writePacket(packet);
}
