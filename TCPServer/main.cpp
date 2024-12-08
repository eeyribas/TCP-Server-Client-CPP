#include "mainwindow.h"

#include <QApplication>
#include "communication.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Communication* communication = new Communication();
    communication->Start(1);

    MainWindow w;
    w.show();
    return a.exec();
}
