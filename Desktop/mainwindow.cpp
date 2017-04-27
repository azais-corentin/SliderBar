#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pStatus = new QLabel;
    ui->statusBar->addWidget(m_pStatus);

    m_pSerial = new QSerialPort(this);
    m_pSerialSettings = new SerialSettings;
    m_serialSettingsLoaded = false;

    initActionsConnections();

    if (m_Settings.value("autoConnect", false).toBool())
    {
        ui->actionAuto_connect->setChecked(true);
        loadSerialConfig();
        m_serialSettingsLoaded = true;
        openSerialPort();
    }
}

MainWindow::~MainWindow()
{
    delete m_pSerialSettings;
    delete ui;
}

void MainWindow::openSerialPort()
{
    if (!m_serialSettingsLoaded)
    {
        SerialSettings::Settings p = m_pSerialSettings->settings();
        m_pSerial->setPortName(p.name);
        m_pSerial->setBaudRate(p.baudRate);
        m_pSerial->setDataBits(p.dataBits);
        m_pSerial->setParity(p.parity);
        m_pSerial->setStopBits(p.stopBits);
        m_pSerial->setFlowControl(p.flowControl);
        m_serialSettingsLoaded = true;
    }
    if (m_pSerial->open(QIODevice::ReadWrite))
    {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        showStatusMessage(tr("Connected to serial: %1")
                          .arg(m_pSerial->portName()));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), m_pSerial->errorString());
        showStatusMessage(tr("Error: Can't open serial port"));
    }
}

void MainWindow::closeSerialPort()
{

}

void MainWindow::writeData(const QByteArray& data)
{

}

void MainWindow::readData()
{

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{

}

void MainWindow::showConfiguration()
{

}

void MainWindow::saveSerialConfig()
{
    m_pSerialSettings->settings()
}

void MainWindow::loadSerialConfig()
{

}

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::showConfiguration);
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_pStatus->setText(message);
}
