#include "spos_mainwindow.h"
#include "ui_spos_mainwindow.h"
#include "../src/dialog/dialog_addproduct.h"
#include "../src/dialog/dialog_modifyproduct.h"
#include "../src/dialog/dialog_deleteproduct.h"
#include "../src/dialog/dialog_findproduct.h"
#include "../src/product/product_mgr.h"
#include <QMessageBox>

#include <QDirIterator>
#include <QDebug>
#include <QIcon>

/**
 * @brief Constructor of the main application window.
 *
 * Initializes the main window UI components and configures the products
 * table used for the current sale. The table columns are configured with
 * product information such as barcode, name, quantity and prices.
 *
 * @param parent Parent widget.
 */
SPOS_MainWindow::SPOS_MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SPOS_MainWindow)
{
    ui->setupUi(this);


    ui->pushButton_NewSale->setIcon(
        QIcon(":/resources/icons8-sell-100.png"));

    ui->pushButton_RemoveProduct->setIcon(
        QIcon(":/resources/icons8-minus-96.png"));

    ui->pushButton_CancelSale->setIcon(
        QIcon(":/resources/icons8-delete-96.png"));

    ui->pushButton_FindProduct->setIcon(
        QIcon(":/resources/icons8-find-96.png"));

    ui->action_AddNewProduct->setIcon(
        QIcon(":/resources/icons8-add-96.png"));

    ui->action_ModifyProduct->setIcon(
        QIcon(":/resources/icons8-pencil-96.png"));

    ui->action_DeleteProduct->setIcon(
        QIcon(":/resources/icons8-remove-80.png"));

    connect(ui->tableWidget_Products,
            &QTableWidget::cellChanged,
            this,
            &SPOS_MainWindow::onSaleItemChanged);

    ui->tableWidget_Products->setColumnCount(6);

    ui->tableWidget_Products->setHorizontalHeaderLabels({
        "ID",
        "Codigo de Barras",
        "Producto",
        "Cantidad",
        "Precio Unitario",
        "Subtotal"
    });

    ui->tableWidget_Products->setColumnWidth(2, 400);

    ui->tableWidget_Products->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Destructor of the main application window.
 *
 * Releases the resources allocated for the UI.
 */
SPOS_MainWindow::~SPOS_MainWindow()
{
    delete ui;
}

/**
 * @brief Opens the dialog to add a new product.
 *
 * Displays the Add Product dialog where the user can register a new product
 * in the database. After closing the dialog, the barcode input field is
 * focused again to continue scanning products.
 */
void SPOS_MainWindow::on_action_AddNewProduct_triggered()
{
    Dialog_AddProduct dialog(this);

    dialog.exec();

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Opens the dialog to modify an existing product.
 *
 * Displays the Modify Product dialog that allows the user to search and
 * update product information stored in the database.
 */
void SPOS_MainWindow::on_action_ModifyProduct_triggered()
{
    Dialog_ModifyProduct dialog(this);

    dialog.exec();

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Opens the dialog to delete a product.
 *
 * Displays the Delete Product dialog where the user can search and remove
 * an existing product from the database.
 */
void SPOS_MainWindow::on_action_DeleteProduct_triggered()
{
    Dialog_DeleteProduct dialog(this);

    dialog.exec();

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Opens the product search dialog.
 *
 * Displays the Find Product dialog that allows the user to search products
 * using different criteria.
 */
void SPOS_MainWindow::on_pushButton_FindProduct_clicked()
{
    Dialog_FindProduct dialog(this);

    connect(
        &dialog,
        &Dialog_FindProduct::productSelected,
        this,
        [this](const Product& product)
        {
            m_sale.addProduct(product);

            updateSaleTable();

            updateSaleTotal();
        }
        );

    dialog.exec();

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Handles barcode scanner input.
 *
 * Retrieves the barcode entered in the barcode input field, searches the
 * corresponding product in the database and adds it to the current sale.
 *
 * If the product already exists in the sale table, its quantity is increased
 * instead of creating a duplicated row.
 */
void SPOS_MainWindow::on_lineEdit_Barcode_returnPressed()
{
    QString barcode = ui->lineEdit_Barcode->text().trimmed();

    if(barcode.isEmpty())
        return;

    QList<Product> products =
        ProductManager::instance().findProducts(barcode, "");

    if(!products.isEmpty())
    {
        m_sale.addProduct(products.first());
        updateSaleTable();
    }

    updateSaleTotal();

    ui->lineEdit_Barcode->clear();
    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Updates the products table with the current sale items.
 *
 * Clears the current sale table and reloads it using the products stored
 * in the active Sale object. Each row displays the product information,
 * including barcode, name, quantity, unit price and subtotal.
 *
 * This function only updates the user interface representation of the sale.
 * The sale data is managed by the Sale class.
 */
void SPOS_MainWindow::updateSaleTable()
{
    ui->tableWidget_Products->blockSignals(true);

    ui->tableWidget_Products->setRowCount(0);

    for(const SaleItem& item : m_sale.getItems())
    {
        int row = ui->tableWidget_Products->rowCount();

        ui->tableWidget_Products->insertRow(row);

        ui->tableWidget_Products->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::number(item.product.id)
                )
            );

        ui->tableWidget_Products->setItem(
            row,
            1,
            new QTableWidgetItem(
                item.product.barcode
                )
            );

        ui->tableWidget_Products->setItem(
            row,
            2,
            new QTableWidgetItem(
                item.product.name
                )
            );

        ui->tableWidget_Products->setItem(
            row,
            3,
            new QTableWidgetItem(
                QString::number(item.quantity)
                )
            );

        ui->tableWidget_Products->setItem(
            row,
            4,
            new QTableWidgetItem(
                QString::number(item.product.sale_price, 'f', 2)
                )
            );

        ui->tableWidget_Products->setItem(
            row,
            5,
            new QTableWidgetItem(
                QString::number(
                    item.quantity * item.product.sale_price,
                    'f',
                    2
                    )
                )
            );

        ui->tableWidget_Products->item(row, 0)->setFlags(
            Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_Products->item(row, 1)->setFlags(
            Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_Products->item(row, 2)->setFlags(
            Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_Products->item(row, 5)->setFlags(
            Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidget_Products->item(row, 3)->setFlags(
            Qt::ItemIsSelectable |
            Qt::ItemIsEnabled |
            Qt::ItemIsEditable);

        ui->tableWidget_Products->item(row, 4)->setFlags(
            Qt::ItemIsSelectable |
            Qt::ItemIsEnabled |
            Qt::ItemIsEditable);

        ui->tableWidget_Products->item(row,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_Products->item(row,1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidget_Products->item(row,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_Products->item(row,3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_Products->item(row,4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget_Products->item(row,5)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }

    ui->tableWidget_Products->blockSignals(false);
}

/**
 * @brief Calculates and updates the total amount of the current sale.
 *
 * Iterates through all products in the sale table, adds their subtotals and
 * displays the final amount in the total field.
 */
void SPOS_MainWindow::updateSaleTotal()
{
    double totalSale = m_sale.getTotal();

    ui->lineEdit_Total->setText(
        QString("$ %1").arg(totalSale, 0, 'f', 2)
        );
}

/**
 * @brief Calculates and displays the customer's change.
 *
 * Retrieves the amount of money received from the customer, converts it
 * from text to a numeric value and calculates the difference between the
 * received amount and the total sale amount.
 *
 * The calculated change is displayed in the exchange field. After the
 * calculation, the barcode input field is focused again to continue the
 * sales process.
 *
 * If the received amount field is empty, the function returns without
 * performing any calculation.
 */
void SPOS_MainWindow::on_lineEdit_Received_returnPressed()
{
    QString receivedText = ui->lineEdit_Received->text().trimmed();

    if(receivedText.isEmpty())
        return;

    double receivedMoney = receivedText.toDouble();

    if(receivedMoney >= m_sale.getTotal())
    {
        double exchange = receivedMoney - m_sale.getTotal();
        ui->lineEdit_Exchange->setText(
            QString("$ %1").arg(exchange, 0, 'f', 2));
    }
    else
    {
        QMessageBox::warning(
            this,
            "Falta Dinero",
            "Falta dinero para cubrir la cuenta a pagar."
            );
    }

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Starts a new sale transaction.
 *
 * Clears the current sale data, refreshes the sale table, updates the
 * displayed total amount and resets the sale-related user interface to
 * prepare for a new transaction.
 */
void SPOS_MainWindow::on_pushButton_NewSale_clicked()
{
    m_sale.clear();
    updateSaleTable();
    updateSaleTotal();
    resetSaleUI();
}

/**
 * @brief Cancels the current sale.
 *
 * Clears all products from the active sale, refreshes the sale table,
 * updates the total amount and resets the sale-related user interface
 * fields to their initial state.
 */
void SPOS_MainWindow::on_pushButton_CancelSale_clicked()
{
    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Cancelar venta",
            "¿Desea cancelar la venta?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

    if (reply == QMessageBox::Yes)
    {
        m_sale.clear();
        updateSaleTable();
        updateSaleTotal();
        resetSaleUI();
    }
}

/**
 * @brief Resets the sale user interface.
 *
 * Clears all sale-related input fields, including the total amount,
 * received money, exchange and barcode fields. Finally, sets the input
 * focus to the barcode field to prepare for the next operation.
 */
void SPOS_MainWindow::resetSaleUI()
{
    ui->lineEdit_Total->clear();
    ui->lineEdit_Received->clear();
    ui->lineEdit_Exchange->clear();
    ui->lineEdit_Barcode->clear();

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Removes the selected product from the current sale.
 *
 * Retrieves the selected product from the sale table and removes it from
 * the active sale using its unique product identifier. After removing the
 * product, the sale table and total amount are refreshed.
 *
 * If no product is selected, a warning message is displayed and no action
 * is performed.
 */
void SPOS_MainWindow::on_pushButton_RemoveProduct_clicked()
{
    int row = ui->tableWidget_Products->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "Remove Product",
            "Please select a product."
            );

        return;
    }

    int productId =
        ui->tableWidget_Products->item(row, 0)->text().toInt();

    m_sale.removeProduct(productId);
    updateSaleTable();
    updateSaleTotal();
    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Handles changes made to sale items in the products table.
 *
 * This function is triggered when a cell value in the current sale table
 * is modified. It processes changes only from the quantity and unit price
 * columns.
 *
 * When the quantity is modified, the corresponding sale item quantity is
 * updated in the Sale object. When the unit price is modified, the product
 * sale price is updated.
 *
 * After a successful update, the row subtotal is recalculated and the total
 * amount of the current sale is refreshed.
 *
 * @param row Row index of the modified item in the products table.
 * @param column Column index of the modified cell.
 */
void SPOS_MainWindow::onSaleItemChanged(int row, int column)
{
    if (column != 3 && column != 4)
        return;

    int productId =
        ui->tableWidget_Products
            ->item(row, 0)
            ->text()
            .toInt();

    double quantity =
        ui->tableWidget_Products
            ->item(row, 3)
            ->text()
            .toDouble();

    double price =
        ui->tableWidget_Products
            ->item(row, 4)
            ->text()
            .toDouble();

    if (column == 3)
    {
        if (!m_sale.updateQuantity(productId, quantity))
            return;
    }
    else
    {
        if (!m_sale.updateUnitPrice(productId, price))
            return;
    }

    double subtotal = quantity * price;

    ui->tableWidget_Products->blockSignals(true);

    ui->tableWidget_Products
        ->item(row, 5)
        ->setText(
            QString::number(subtotal, 'f', 2)
            );

    ui->tableWidget_Products->blockSignals(false);

    updateSaleTotal();

    ui->lineEdit_Barcode->setFocus();
}