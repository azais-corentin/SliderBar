#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("SliderBar");
    QCoreApplication::setOrganizationDomain("github.com/Haellsigh/SliderBar/");
    QCoreApplication::setApplicationName("SliderBar");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
