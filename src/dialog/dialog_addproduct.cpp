#include "dialog_addproduct.h"
#include "ui_dialog_addproduct.h"

Dialog_AddProduct::Dialog_AddProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_AddProduct)
{
    ui->setupUi(this);
}

Dialog_AddProduct::~Dialog_AddProduct()
{
    delete ui;
}
