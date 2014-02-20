#include "pndialog.h"
#include <QApplication>

#ifndef VERSION
#define VERSION "0.0.5"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("kiz");
    a.setApplicationName("PostNot-Tray");
    a.setOrganizationDomain("rz.uni-ylm.de");
    a.setApplicationVersion(VERSION);

    PNDialog w;

    // TODO:
    // Beim Ersten Start die Konfigurationzeigen, dann nicht mehr.
    //    w.show();

    return a.exec();
}
