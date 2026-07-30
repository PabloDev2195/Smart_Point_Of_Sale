#include "dialog_findproduct.h"
#include "ui_dialog_findproduct.h"
#include "../product/product_mgr.h"
#include <QHeaderView>
#include <QMessageBox>

/**
 * @brief Constructs the Find Product dialog.
 *
 * Initializes the user interface, configures the products table model,
 * and prepares the view for displaying search results.
 *
 * @param parent Parent widget.
 */
Dialog_FindProduct::Dialog_FindProduct(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_FindProduct)
    , m_model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    m_model->setHorizontalHeaderLabels({
        "ID",
        "Codigo de Barras",
        "Producto",
        "Precio de compra",
        "Precio de venta"
    });

    ui->tableView_FindProduct->setModel(m_model);

    ui->tableView_FindProduct->horizontalHeader()->setStretchLastSection(true);

    ui->tableView_FindProduct->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);
}

/**
 * @brief Destroys the Find Product dialog.
 *
 * Releases the user interface resources.
 */
Dialog_FindProduct::~Dialog_FindProduct()
{
    delete ui;
}

/**
 * @brief Searches for products using the specified barcode and/or name.
 *
 * Retrieves the matching products from ProductManager, clears the
 * search fields, and displays the results in the table view.
 */
void Dialog_FindProduct::on_pushButton_FindProduct_clicked()
{
    QString barcode = ui->lineEdit_Barcode->text();
    QString name = ui->lineEdit_Name->text();

    m_products =
        ProductManager::instance().findProducts(barcode, name);

    ui->lineEdit_Barcode->clear();
    ui->lineEdit_Name->clear();

    loadProducts(m_products);
}

/**
 * @brief Populates the products table with search results.
 *
 * Clears the current table contents and inserts the specified
 * products into the table model.
 *
 * @param products List of products to display.
 */
void Dialog_FindProduct::loadProducts(const QList<Product>& products)
{
    m_model->removeRows(0, m_model->rowCount());

    for(const Product &product : products)
    {
        QList<QStandardItem*> row;

        row << new QStandardItem(QString::number(product.id));
        row << new QStandardItem(product.barcode);
        row << new QStandardItem(product.name);
        row << new QStandardItem(QString::number(product.purchase_price));
        row << new QStandardItem(QString::number(product.sale_price));

        m_model->appendRow(row);
    }
}

/**
 * @brief Adds the selected product to the current sale.
 *
 * Validates that a product has been selected, retrieves the
 * corresponding Product object, emits the productSelected()
 * signal, and closes the dialog.
 *
 * Displays a warning message if no product is selected.
 */
void Dialog_FindProduct::on_pushButton_AddProduct_clicked()
{
    QModelIndex index =
        ui->tableView_FindProduct->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(
            this,
            "Select Product",
            "Please select a product."
            );

        return;
    }

    int row = index.row();

    int productId =
        m_model->item(row, 0)
            ->text()
            .toInt();

    for(const Product& product : m_products)
    {
        if(product.id == productId)
        {
            emit productSelected(product);

            close();

            return;
        }
    }
}
