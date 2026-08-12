#include "spos_mainwindow.h"
#include "ui_spos_mainwindow.h"
#include "../src/dialog/dialog_addproduct.h"
#include "../src/dialog/dialog_modifyproduct.h"
#include "../src/dialog/dialog_deleteproduct.h"
#include "../src/dialog/dialog_findproduct.h"
#include "../src/dialog/dialog_payment.h"
#include "../src/product_database/product_mgr.h"
#include "../src/sale_database/sale_database.h"
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

    ui->pushButton_RemoveProduct->setIcon(
        QIcon(":/resources/icons8-minus-96.png"));

    ui->pushButton_CancelSale->setIcon(
        QIcon(":/resources/icons8-delete-96.png"));

    ui->pushButton_FindProduct->setIcon(
        QIcon(":/resources/icons8-find-96.png"));

    ui->pushButton_AddNewProduct->setIcon(
        QIcon(":/resources/icons8-add-96.png"));

    ui->pushButton_ModifyProduct->setIcon(
        QIcon(":/resources/icons8-pencil-96.png"));

    ui->pushButton_DeleteProduct->setIcon(
        QIcon(":/resources/icons8-remove-80.png"));

    ui->pushButton_Payment->setIcon(
        QIcon(":/resources/icons8-payment-100.png"));

    ui->pushButton_FinishSale->setIcon(
        QIcon(":/resources/icons8-ok-80.png"));

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

    ui->tableWidget_Tickets->setColumnCount(3);

    ui->tableWidget_Tickets->setHorizontalHeaderLabels({
        "ID",
        "Fecha",
        "Total",
    });

    ui->tableWidget_Tickets->setColumnWidth(1, 210);

    ui->tableWidget_Tickets->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=](){
        ui->label_UpdateDate->setText(
            QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")
            );
    });

    timer->start(1000);

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
void SPOS_MainWindow::on_pushButton_AddNewProduct_clicked()
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
void SPOS_MainWindow::on_pushButton_ModifyProduct_clicked()
{
    Dialog_ModifyProduct dialog(this);

    dialog.exec();

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Opens the payment dialog and updates the payment information.
 *
 * Retrieves the current sale total and passes it to the payment dialog.
 * The dialog allows the user to enter the amount received from the customer
 * and calculates the corresponding change.
 *
 * Once the dialog is closed, the received payment and calculated change
 * are displayed in the main window.
 *
 * Finally, the barcode input field is focused again to allow the next
 * product to be scanned.
 */
void SPOS_MainWindow::on_pushButton_Payment_clicked()
{
    double total = m_currentsale.getTotal();

    Dialog_Payment dialog(total, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        ui->label_Payment->setText(
            QString("$ %1").arg(dialog.getPayment(), 0, 'f', 2)
            );

        ui->label_Change->setText(
            QString("$ %1").arg(dialog.getChange(), 0, 'f', 2)
            );
    }

    ui->lineEdit_Barcode->setFocus();
}

/**
 * @brief Opens the dialog to delete a product.
 *
 * Displays the Delete Product dialog where the user can search and remove
 * an existing product from the database.
 */
void SPOS_MainWindow::on_pushButton_DeleteProduct_clicked()
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
            m_currentsale.addProduct(product);

            updateSaleTable();

            updateSaleTotal();
        }
        );

    dialog.exec();

    SaleDatabase saleDatabase;

    int lastTicket = saleDatabase.getLastTicketNumber();

    ui->label_UpdateSale->setText(
        QString("%1").arg(lastTicket, 6, 10, QChar('0'))
        );

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
        m_currentsale.addProduct(products.first());
        updateSaleTable();
    }

    updateSaleTotal();

    SaleDatabase saleDatabase;

    int lastTicket = saleDatabase.getLastTicketNumber();

    ui->label_UpdateSale->setText(
        QString("%1").arg(lastTicket, 6, 10, QChar('0'))
        );

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

    for(const SaleItem& item : m_currentsale.getItems())
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
    double totalSale = m_currentsale.getTotal();

    ui->label_TotalNum->setText(
        QString("$ %1").arg(totalSale, 0, 'f', 2)
        );
}

/**
 * @brief Starts a new sale transaction.
 *
 * Clears the current sale data, refreshes the sale table, updates the
 * displayed total amount and resets the sale-related user interface to
 * prepare for a new transaction.
 */
void SPOS_MainWindow::on_pushButton_FinishSale_clicked()
{
    if(m_currentsale.getTotal() <= 0)
    {
        return;
    }

    SaleDatabase saleDatabase;

    if(!saleDatabase.saveSale(m_currentsale))
    {
        qDebug() << "Failed to save sale.";
        return;
    }

    int lastTicket = saleDatabase.getLastTicketNumber();

    ui->label_UpdateSale->setText(
        QString("%1").arg(lastTicket, 6, 10, QChar('0'))
        );

    m_currentsale.clear();
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
        m_currentsale.clear();
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
    ui->label_TotalNum->setText(
        QString("$ 0.00"));
    ui->label_Change->setText(
        QString("$ 0.00"));
    ui->label_Payment->setText(
        QString("$ 0.00"));
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

    m_currentsale.removeProduct(productId);
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
        if (!m_currentsale.updateQuantity(productId, quantity))
            return;
    }
    else
    {
        if (!m_currentsale.updateUnitPrice(productId, price))
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