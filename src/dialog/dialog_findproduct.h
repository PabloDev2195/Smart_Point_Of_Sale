#ifndef DIALOG_FINDPRODUCT_H
#define DIALOG_FINDPRODUCT_H

#include <QDialog>
#include <QStandardItemModel>
#include "../models/Product.h"

namespace Ui {
class Dialog_FindProduct;
}

class Dialog_FindProduct : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_FindProduct(QWidget *parent = nullptr);
    ~Dialog_FindProduct();

private slots:
    void on_pushButton_FindProduct_clicked();

    void on_pushButton_AddProduct_clicked();

private:
    Ui::Dialog_FindProduct *ui;

    QStandardItemModel *m_model;

    QList<Product> m_products;

    void loadProducts(const QList<Product>& products);

signals:

    /**
     * @brief Emitted when a product is selected to be added to the sale.
     *
     * @param product Selected product.
     */
    void productSelected(const Product& product);
};

#endif // DIALOG_FINDPRODUCT_H
