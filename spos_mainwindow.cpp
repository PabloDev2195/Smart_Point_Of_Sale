#include "spos_mainwindow.h"
#include "ui_spos_mainwindow.h"

SPOS_MainWindow::SPOS_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPOS_MainWindow)
{
    ui->setupUi(this);
}

SPOS_MainWindow::~SPOS_MainWindow()
{
    delete ui;
}
