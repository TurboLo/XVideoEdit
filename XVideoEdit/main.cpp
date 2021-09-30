#include "XVideoUi.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XVideoUi w;
    w.show();
    return a.exec();
}
