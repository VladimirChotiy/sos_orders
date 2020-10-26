#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SOS");
    QCoreApplication::setOrganizationDomain("sos-dn.net");
    QCoreApplication::setApplicationName("Orders");
    MainWindow w;
    w.show();
    return a.exec();
}
