#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("IndiAltAzDerotator");
    QCoreApplication::setOrganizationDomain("indialtazderotator.org");
    QCoreApplication::setApplicationName("Indi Alt-Az Derotator");

    MainWindow w;
    w.show();
    return a.exec();
}
