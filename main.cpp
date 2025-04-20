#include "mainwindow.h"
#include "simulator.hpp"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Simulator simulator{};
    MainWindow w(simulator);
    a.setWindowIcon(QIcon("icon.png"));
    w.show();
    return a.exec();
}
