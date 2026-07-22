#include "spos_mainwindow.h"
#include "ui_spos_mainwindow.h"
#include "../src/dialog/dialog_addproduct.h"
#include "../src/dialog/dialog_modifyproduct.h"
#include "../src/dialog/dialog_deleteproduct.h"

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

void SPOS_MainWindow::on_action_AddNewProduct_triggered()
{
    Dialog_AddProduct dialog(this);

    dialog.exec();
}

void SPOS_MainWindow::on_action_ModifyProduct_triggered()
{
    Dialog_ModifyProduct dialog(this);

    dialog.exec();
}

void SPOS_MainWindow::on_action_DeleteProduct_triggered()
{
    Dialog_DeleteProduct dialog(this);

    dialog.exec();
}