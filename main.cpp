#include "spos_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SPOS_MainWindow w;
    w.show();
    return QApplication::exec();
}
