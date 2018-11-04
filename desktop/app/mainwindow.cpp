#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sliderbar/sliderbar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    actionPlugins = new QAction("Plugins");
    ui->menuBar->addAction(actionPlugins);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete actionPlugins;
}

void MainWindow::initialiseActions()
{
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->actionConnect, &QAction::triggered, m_sliderbar, &SliderBar::connect);
    connect(ui->actionAutoconnect, &QAction::triggered, m_sliderbar, &SliderBar::autoconnect);
    connect(ui->actionSettings, &QAction::triggered, m_sliderbar, &SliderBar::settings);
}
