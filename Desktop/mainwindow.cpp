#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialprotocol.h"

#include <QDebug>
#include <QLabel>
#include <QKeySequence>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_pSystemKeyboardHook = SystemKeyboardHook::instance();
    m_pSystemKeyboardHook->setConnected(true);

    ui->setupUi(this);
    m_pStatus = new QLabel;
    ui->statusBar->addWidget(m_pStatus);
    enableConnect();

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

    //connect(m_pSystemKeyboardHook, &SystemKeyboardHook::keyPressed, this, &MainWindow::handleKeyPressed);
    connect(m_pSystemKeyboardHook, SIGNAL(keyPressed(DWORD)), this, SLOT(handleKeyPressed(DWORD)));
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_pStatus->setText(message);
}

void MainWindow::on_eProgress_valueChanged(int value)
{
    command cmd;
    cmd.type = command::FORS_POSITION;
    cmd.value = static_cast<uint16_t>(value);
    m_pSerial->writePacket(cmd);
}

void MainWindow::handleKeyPressed(DWORD key)
{
    QKeySequence seq(key);
    showStatusMessage("Key pressed: " + seq.toString() + ", " + QString::number(key));
}
