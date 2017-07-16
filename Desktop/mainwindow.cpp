#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serialprotocol.h"

#include <QDebug>
#include <QLabel>
#include <QKeySequence>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //m_pSystemKeyboardHook = SystemKeyboardHook::instance();
    //m_pSystemKeyboardHook->setConnected(true);

    ui->setupUi(this);
    m_pStatus = new QLabel;
    ui->statusBar->addWidget(m_pStatus);
    enableConnect();

    m_pSerial = new SerialProtocol;
    m_pSettingsDialog = new SettingsDialog(this);

    m_pTimerStatusMessage = new QTimer;
    m_pTimerStatusMessage->setSingleShot(true);
    m_pTimerStatusMessage->setInterval(2000);

    initConnections();
    loadSettings();

    if (m_settings.value("serial/autoconnect", false).toBool())
        openSerialPort();

    //Setup plot
    ui->dataPlot->clearGraphs();
    // Position
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    // Target position
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(1)->setPen(QPen(QColor(40, 200, 40)));
    // Estimated position
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(2)->setPen(QPen(QColor(0, 0, 255)));
    // Estimated velocity
    ui->dataPlot->addGraph();
    ui->dataPlot->graph(3)->setPen(QPen(QColor(70, 0, 70)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%z");
    ui->dataPlot->xAxis->setTicker(timeTicker);
    ui->dataPlot->yAxis->setRange(0, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pTimerStatusMessage;

    delete m_pSystemKeyboardHook;
    delete m_pSerial;
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

void MainWindow::receivePacket(const command& packet)
{
    m_count++;
    static QTime time(QTime::currentTime());

    switch (packet.type)
    {
        case command::FORC_POSITION:
        {
            m_sliderPos = packet.value;
            double key = time.elapsed() / 1000.0;
            ui->progressBar->setValue(int(0.0975879 * double(packet.value) + 0.2651662));
            ui->dataPlot->graph(0)->addData(key, 0.0975879 * double(packet.value) + 0.2651662);
            ui->dataPlot->graph(1)->addData(key, 0.0975879 * double(ui->eProgress->value()) + 0.2651662);
            ui->dataPlot->xAxis->setRange(key, 4, Qt::AlignRight);
            ui->dataPlot->replot();
            break;
        }
        case command::FORC_EST_POS:
        {
            double key = time.elapsed() / 1000.0;
            ui->dataPlot->graph(2)->addData(key, 0.0975879 * double(packet.value) + 0.2651662);
            ui->dataPlot->xAxis->setRange(key, 4, Qt::AlignRight);
            ui->dataPlot->replot();
            break;
        }
        case command::FORC_EST_VEL:
        {
            double key = time.elapsed() / 1000.0;
            ui->dataPlot->graph(3)->addData(key, 0.0975879 * double(packet.value));
            ui->dataPlot->xAxis->setRange(key, 4, Qt::AlignRight);
            ui->dataPlot->replot();
            break;
        }
        default:
            qDebug() << "Error: Packet not meant for computer!";
            break;
    }
    showStatusMessage(QString::number(m_count));
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

    connect(m_pTimerStatusMessage, &QTimer::timeout, this, &MainWindow::clearStatusMessage);
    //connect(m_pSystemKeyboardHook, &SystemKeyboardHook::keyPressed, this, &MainWindow::handleKeyPressed);
}

void MainWindow::showStatusMessage(const QString& message)
{
    m_pStatus->setText(message);
    m_pTimerStatusMessage->start(1000 + message.split(' ').length() * 800);
}

void MainWindow::clearStatusMessage()
{
    m_pStatus->clear();
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

void MainWindow::on_bAddResist_clicked()
{
    command cmd;
    cmd.type = command::FORS_RESIST_AT;
    cmd.value = static_cast<uint16_t>(ui->eResistAt->value());
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bClearResists_clicked()
{
    command cmd;
    cmd.type = command::FORS_RESIST_CLEAR;
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bDisablePID_clicked()
{
    command cmd;
    cmd.type = command::FORS_STOP_PID;
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bEnablePID_clicked()
{
    command cmd;
    cmd.type = command::FORS_START_PID;
    m_pSerial->writePacket(cmd);
}

void MainWindow::on_bUpdatePID_clicked()
{
    double pg = ui->epGain->value();
    double ig = ui->eiGain->value();
    double dg = ui->edGain->value();

    command pcmd;
    pcmd.type = command::FORS_PID_P;
    pcmd.value = static_cast<uint16_t>(pg * 327.68);
    m_pSerial->writePacket(pcmd);

    command icmd;
    icmd.type = command::FORS_PID_I;
    icmd.value = static_cast<uint16_t>(ig * 327.68);
    m_pSerial->writePacket(icmd);

    command dcmd;
    dcmd.type = command::FORS_PID_D;
    dcmd.value = static_cast<uint16_t>(dg * 327.68);
    m_pSerial->writePacket(dcmd);
}
