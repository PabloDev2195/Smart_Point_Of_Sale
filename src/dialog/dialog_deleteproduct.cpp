#include "dialog_deleteproduct.h"
#include "ui_dialog_deleteproduct.h"

Dialog_DeleteProduct::Dialog_DeleteProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_DeleteProduct)
{
    ui->setupUi(this);
}

Dialog_DeleteProduct::~Dialog_DeleteProduct()
{
    delete ui;
}
