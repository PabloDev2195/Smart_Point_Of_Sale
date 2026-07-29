#include "dialog_findproduct.h"
#include "ui_dialog_findproduct.h"
#include "../product/product_mgr.h"
#include <QHeaderView>

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

Dialog_FindProduct::~Dialog_FindProduct()
{
    delete ui;
}

void Dialog_FindProduct::on_pushButton_FindProduct_clicked()
{
    QString barcode = ui->lineEdit_Barcode->text();
    QString name = ui->lineEdit_Name->text();

    QList<Product> products =
        ProductManager::instance().findProducts(barcode, name);

    ui->lineEdit_Barcode->clear();
    ui->lineEdit_Name->clear();

    loadProducts(products);
}

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