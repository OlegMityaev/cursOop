#include "MainWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setGeometry(300, 400, 800, 400);
    w.show();
    return a.exec();
}
