#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SOS");
    QCoreApplication::setOrganizationDomain("sos-dn.net");
    QCoreApplication::setApplicationName("Orders");
    QCoreApplication::setApplicationVersion("0.1.2");
    MainWindow w;
    w.show();
    return a.exec();
}
