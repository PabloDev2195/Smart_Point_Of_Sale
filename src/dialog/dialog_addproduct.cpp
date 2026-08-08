#include "dialog_addproduct.h"
#include "ui_dialog_addproduct.h"
#include "../models/Product.h"
#include "../product_database/product_mgr.h"
#include <QMessageBox>

/**
 * @brief Constructs the Add Product dialog.
 *
 * Initializes the user interface for creating a new product.
 *
 * @param parent Parent widget.
 */
Dialog_AddProduct::Dialog_AddProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_AddProduct)
{
    ui->setupUi(this);
}

/**
 * @brief Destroys the Add Product dialog.
 *
 * Releases the user interface resources.
 */
Dialog_AddProduct::~Dialog_AddProduct()
{
    delete ui;
}

/**
 * @brief Saves a new product.
 *
 * Builds the product name using the supplier, product name, and unit fields,
 * validates the required information, and attempts to store the product in
 * the database through ProductManager.
 *
 * If the operation succeeds, the dialog is closed with an accepted result.
 * Otherwise, an error message is displayed.
 */
void Dialog_AddProduct::on_pushButton_Save_clicked()
{
    QString provider = ui->lineEdit_Supplier->text().trimmed();
    QString name = ui->lineEdit_Name->text().trimmed();
    QString unit = ui->lineEdit_Unit->text().trimmed();

    QString fullName = provider + " / " + name + " / " + unit;

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

/**
 * @brief Cancels the product creation.
 *
 * Closes the dialog without saving any information.
 */
void Dialog_AddProduct::on_pushButton_Cancel_clicked()
{
    reject();
}

