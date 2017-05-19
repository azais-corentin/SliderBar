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
    enableConnect();

    m_pTimer_fps = new QTimer(this);
    m_frames = 0;
    connect(m_pTimer_fps, SIGNAL(timeout()), this, SLOT(updateFPS()));
    m_pTimer_fps->start(1000);

    m_pSerial = new SerialProtocol;
    m_pSettingsDialog = new SettingsDialog(this);
    initConnections();
    loadSettings();

    if (m_settings.value("serial/autoconnect", false).toBool())
        openSerialPort();
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
    m_frames++;
    switch (packet.type)
    {
        case command::FORC_POSITION:
            m_sliderPos = packet.value;
            ui->progressBar->setValue(int(0.0975879f * float(packet.value) + 0.42659893f));
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

void MainWindow::updateFPS()
{
    showStatusMessage("Fps: " + QString::number(m_frames));
    m_frames = 0;
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

void MainWindow::on_bLeft_clicked()
{
    command leftcmd;
    leftcmd.type = command::FORS_POSITION;
    leftcmd.value = static_cast<uint16_t>(m_sliderPos - 100 <= 0 ? 0 : m_sliderPos - 100);
    m_pSerial->writePacket(leftcmd);
}

void MainWindow::on_bStop_clicked()
{
    command stopcmd;
    stopcmd.type = command::FORS_SPEED;
    stopcmd.value = 32768;
    m_pSerial->writePacket(stopcmd);
}

void MainWindow::on_bRight_clicked()
{
    command rightcmd;
    rightcmd.type = command::FORS_POSITION;
    rightcmd.value = static_cast<uint16_t>(m_sliderPos - 100 >= 1022 ? 1022 : m_sliderPos + 100);
    m_pSerial->writePacket(rightcmd);
}
