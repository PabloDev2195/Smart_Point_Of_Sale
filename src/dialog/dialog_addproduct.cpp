#include "dialog_addproduct.h"
#include "ui_dialog_addproduct.h"
#include "../models/Product.h"
#include "../product/product_mgr.h"
#include <QMessageBox>

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

    if (product.name.isEmpty())
    {
        QMessageBox::warning(this,
                             "Información Incompleta",
                             "Por favor ingresa el nombre del producto.");
        ui->lineEdit_Name->setFocus();
        return;
    }

    if (product.purchase_price <= 0)
    {
        QMessageBox::warning(this,
                             "Información Incompleta",
                             "Por favor ingresa el precio de compra.");
        ui->doubleSpinBox_PurchasePrice->setFocus();
        return;
    }

    if (product.sale_price <= 0)
    {
        QMessageBox::warning(this,
                             "Información Incompleta",
                             "Por favor ingresa el precio de venta.");
        ui->doubleSpinBox_SellPrice->setFocus();
        return;
    }

    if (ProductManager::instance().addProduct(product))
    {
        QMessageBox::information(
            this,
            "Producto Agregado",
            "El producto se guardó correctamente."
            );

        accept();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Error",
            "El producto no pudo ser agregado."
            );
    }
}


void Dialog_AddProduct::on_pushButton_Cancel_clicked()
{
    reject();
}

