#include "spos_mainwindow.h"
#include "ui_spos_mainwindow.h"
#include "../src/dialog/dialog_addproduct.h"
#include "../src/dialog/dialog_modifyproduct.h"
#include "../src/dialog/dialog_deleteproduct.h"
#include "../src/dialog/dialog_findproduct.h"

SPOS_MainWindow::SPOS_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPOS_MainWindow)
    , m_model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    m_model->setHorizontalHeaderLabels({
        "ID",
        "Codigo de Barras",
        "Producto",
        "Precio de compra",
        "Precio de venta"
    });

    ui->tableView_Products->setModel(m_model);

    ui->tableView_Products->horizontalHeader()->setStretchLastSection(true);

    ui->tableView_Products->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);
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

void SPOS_MainWindow::on_pushButton_FindProduct_clicked()
{
    Dialog_FindProduct dialog(this);

    dialog.exec();
}