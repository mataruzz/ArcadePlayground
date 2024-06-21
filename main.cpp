#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon icon_app("://Images/arcade_platform.png");
    w.setWindowIcon(icon_app);
    w.show();
    return a.exec();
}
