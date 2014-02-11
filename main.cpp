#include "pndialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PNDialog w;
//    w.show();
    return a.exec();
}
