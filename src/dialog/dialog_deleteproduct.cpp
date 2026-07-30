#include "dialog_deleteproduct.h"
#include "ui_dialog_deleteproduct.h"
#include "../product/product_mgr.h"
#include <QMessageBox>

/**
 * @brief Constructs the Delete Product dialog.
 *
 * Initializes the user interface, configures the products table,
 * and prepares the dialog for searching and deleting products.
 *
 * @param parent Parent widget.
 */
Dialog_DeleteProduct::Dialog_DeleteProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_DeleteProduct)
{
    ui->setupUi(this);

    ui->tableWidget_Products->setColumnCount(5);

    QStringList headers;
    headers << "ID"
            << "Nombre"
            << "Codigo"
            << "Precio de compra"
            << "Precio de venta";

    ui->tableWidget_Products->setHorizontalHeaderLabels(headers);


    ui->tableWidget_Products->setColumnWidth(1, 350);

    ui->tableWidget_Products->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->tableWidget_Products->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );
}

/**
 * @brief Destroys the Delete Product dialog.
 *
 * Releases the user interface resources.
 */
Dialog_DeleteProduct::~Dialog_DeleteProduct()
{
    delete ui;
}

/**
 * @brief Searches for products using the specified barcode and/or name.
 *
 * Retrieves the matching products from ProductManager and displays
 * the results in the products table.
 */
void Dialog_DeleteProduct::on_pushButton_Find_clicked()
{
    QString barcode = ui->lineEdit_Barcode->text();
    QString name = ui->lineEdit_Name->text();

    QList<Product> products =
        ProductManager::instance().findProducts(barcode, name);

    ui->tableWidget_Products->clearContents();
    ui->tableWidget_Products->setRowCount(products.size());

    ui->lineEdit_Barcode->clear();
    ui->lineEdit_Name->clear();

    for (int row = 0; row < products.size(); row++)
    {
        const Product& product = products.at(row);

        ui->tableWidget_Products->setItem(
            row,
            0,
            new QTableWidgetItem(QString::number(product.id))
            );

        ui->tableWidget_Products->setItem(
            row,
            1,
            new QTableWidgetItem(product.name)
            );

        ui->tableWidget_Products->setItem(
            row,
            2,
            new QTableWidgetItem(product.barcode)
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
 * @brief Stores the selected product identifier.
 *
 * Saves the identifier of the product selected in the table so it
 * can be deleted later.
 *
 * @param row Selected table row.
 * @param column Selected table column (unused).
 */
void Dialog_DeleteProduct::on_tableWidget_Products_cellClicked(
    int row,
    int column)
{
    Q_UNUSED(column);

    m_selectedProductId =
        ui->tableWidget_Products
            ->item(row,0)
            ->text()
            .toInt();
}

/**
 * @brief Deletes the selected product.
 *
 * Verifies that a product has been selected, requests user
 * confirmation, and removes the product from the database using
 * ProductManager.
 *
 * Displays a confirmation or error message depending on the result.
 */
void Dialog_DeleteProduct::on_pushButton_Delete_clicked()
{
    if (m_selectedProductId == -1)
    {
        QMessageBox::warning(
            this,
            "No Selección",
            "Por favor selecciona un producto."
            );

        return;
    }


    auto answer = QMessageBox::question(
        this,
        "Eliminar",
        "¿Estás seguro de eliminar este producto?"
        );


    if (answer != QMessageBox::Yes)
        return;


    bool result = ProductManager::instance().deleteProduct(m_selectedProductId);


    if (result)
    {
        QMessageBox::information(
            this,
            "Eliminar",
            "Producto eliminado correctamente."
            );

        close();
    }
    else
    {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo eliminar el producto."
            );
    }
}

/**
 * @brief Cancels the delete operation.
 *
 * Closes the dialog without deleting any product.
 */
void Dialog_DeleteProduct::on_pushButton_Cancel_clicked()
{
    reject();
}
