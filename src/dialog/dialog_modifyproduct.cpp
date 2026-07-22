#include "dialog_modifyproduct.h"
#include "ui_dialog_modifyproduct.h"

Dialog_ModifyProduct::Dialog_ModifyProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_ModifyProduct)
{
    ui->setupUi(this);
}

Dialog_ModifyProduct::~Dialog_ModifyProduct()
{
    delete ui;
}
