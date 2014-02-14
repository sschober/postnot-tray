#include "pndialog.h"
#include <QApplication>

#ifndef VERSION
#define VERSION "0.0.4"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("kiz");
    a.setApplicationName("PostNot-Tray");
    a.setOrganizationDomain("rz.uni-ylm.de");
    a.setApplicationVersion(VERSION);

    PNDialog w;

    //    w.show();

    return a.exec();
}
