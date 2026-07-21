#include "spos_mainwindow.h"
#include "database/database_Manager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SPOS_MainWindow w;

    if(!DatabaseManager::instance().openDatabase())
        return -1;

    if(!DatabaseManager::instance().createTables())
        return -1;

    w.show();
    return QApplication::exec();
}
