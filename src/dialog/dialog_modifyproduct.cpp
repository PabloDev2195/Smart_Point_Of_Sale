#include "dialog_modifyproduct.h"
#include "ui_dialog_modifyproduct.h"
#include "../product_database/product_mgr.h"
#include <QMessageBox>

/**
 * @brief Constructs the Modify Product dialog.
 *
 * Initializes the user interface, configures the products table,
 * and connects the table row selection to the product loading slot.
 *
 * @param parent Parent widget.
 */
Dialog_ModifyProduct::Dialog_ModifyProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_ModifyProduct)
{
    ui->setupUi(this);

    ui->tableWidget_Products->setColumnCount(5);

    QStringList headers;
    headers << "ID"
            << "Código de Barras"
            << "Nombre"
            << "Precio Compra"
            << "Precio Venta";


    ui->tableWidget_Products->setHorizontalHeaderLabels(headers);


    ui->tableWidget_Products->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tableWidget_Products->setSelectionMode(
        QAbstractItemView::SingleSelection
        );


    connect(ui->tableWidget_Products,
            &QTableWidget::cellClicked,
            this,
            &Dialog_ModifyProduct::loadSelectedProduct);
}

/**
 * @brief Searches for products using the specified barcode and/or name.
 *
 * Retrieves matching products from the ProductManager and populates
 * the table with the search results.
 */
void Dialog_ModifyProduct::on_pushButton_Find_clicked()
{
    ProductManager& manager = ProductManager::instance();

    QList<Product> products =
        manager.findProducts(
            ui->lineEdit_FindBarcode->text(),
            ui->lineEdit_FindName->text()
            );


    ui->tableWidget_Products->setRowCount(0);


    for(const Product& product : products)
    {
        int row = ui->tableWidget_Products->rowCount();

        ui->tableWidget_Products->insertRow(row);


        ui->tableWidget_Products->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::number(product.id)
                )
            );


        ui->tableWidget_Products->setItem(
            row,
            1,
            new QTableWidgetItem(
                product.barcode
                )
            );


        ui->tableWidget_Products->setItem(
            row,
            2,
            new QTableWidgetItem(
                product.name
                )
            );


        ui->tableWidget_Products->setItem(
            row,
            3,
            new QTableWidgetItem(
                QString::number(product.purchase_price)
                )
            );


        ui->tableWidget_Products->setItem(
            row,
            4,
            new QTableWidgetItem(
                QString::number(product.sale_price)
                )
            );
    }
}

/**
 * @brief Loads the selected product into the editing controls.
 *
 * Retrieves the product associated with the selected table row and
 * displays its information in the input fields, allowing the user
 * to modify its data.
 *
 * @param row Selected table row.
 * @param column Selected column (unused).
 */
void Dialog_ModifyProduct::loadSelectedProduct(int row, int)
{
    int id =
        ui->tableWidget_Products
            ->item(row,0)
            ->text()
            .toInt();


    ProductManager& manager = ProductManager::instance();


    m_product = manager.getProduct(id);


    ui->lineEdit_Name->setText(
        m_product.name
        );


    ui->lineEdit_Barcode->setText(
        m_product.barcode
        );


    ui->doubleSpinBox_PurchasePrice->setValue(
        m_product.purchase_price
        );


    ui->doubleSpinBox_SellPrice->setValue(
        m_product.sale_price
        );
}

/**
 * @brief Saves the modified product information.
 *
 * Reads the values entered by the user, updates the current Product
 * object, and sends the changes to the ProductManager. If the update
 * succeeds, the dialog is closed with an accepted result; otherwise,
 * an error message is displayed.
 */
void Dialog_ModifyProduct::on_pushButton_Save_clicked()
{
    m_product.name = ui->lineEdit_Name->text();
    m_product.barcode = ui->lineEdit_Barcode->text();
    m_product.purchase_price = ui->doubleSpinBox_PurchasePrice->value();
    m_product.sale_price = ui->doubleSpinBox_SellPrice->value();

    ProductManager& manager = ProductManager::instance();

    if (manager.updateProduct(m_product))
    {
        QMessageBox::information(
            this,
            "Producto Actualizado",
            "El producto se actualizó correctamente."
            );

        accept();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo actualizar el producto."
            );
    }
}

/**
 * @brief Cancels the product modification.
 *
 * Closes the dialog without saving any changes.
 */
void Dialog_ModifyProduct::on_pushButton_Cancel_clicked()
{
    reject();
}

/**
 * @brief Destroys the Modify Product dialog.
 *
 * Releases the user interface resources.
 */
Dialog_ModifyProduct::~Dialog_ModifyProduct()
{
    delete ui;
}
