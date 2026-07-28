#include "dialog_addproduct.h"
#include "ui_dialog_addproduct.h"
#include "../models/Product.h"
#include "../product/product_mgr.h"

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

void Dialog_AddProduct::on_pushButton_Save_clicked()
{
    QString provider = ui->lineEdit_Supplier->text().trimmed();
    QString name = ui->lineEdit_Name->text().trimmed();
    QString unit = ui->lineEdit_Unit->text().trimmed();

    QString fullName = provider + " " + name + " " + unit;

    Product product;
    product.name = fullName;
    product.barcode = ui->lineEdit_Barcode->text();
    product.purchase_price = ui->doubleSpinBox_PurchasePrice->value();
    product.sale_price = ui->doubleSpinBox_SellPrice->value();

    ProductManager::instance().addProduct(product);
    accept();
}


void Dialog_AddProduct::on_pushButton_Cancel_clicked()
{
    reject();
}

