#ifndef DIALOG_FINDPRODUCT_H
#define DIALOG_FINDPRODUCT_H

#include <QDialog>
#include <QStandardItemModel>
#include "../models/Product.h"

namespace Ui {
/**
 * @brief Forward declaration of the generated UI class.
 */
class Dialog_FindProduct;
}

/**
 * @brief Dialog used to search and select products.
 *
 * This dialog allows the user to:
 * - Search products by barcode or name.
 * - Display matching products in a table view.
 * - Select a product from the search results.
 * - Notify the caller when a product is selected.
 */
class Dialog_FindProduct : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the Find Product dialog.
     *
     * @param parent Parent widget.
     */
    explicit Dialog_FindProduct(QWidget *parent = nullptr);

    /**
     * @brief Destroys the dialog.
     */
    ~Dialog_FindProduct();

private slots:
    /**
     * @brief Searches for products matching the entered criteria.
     */
    void on_pushButton_FindProduct_clicked();

    /**
     * @brief Emits the selected product and closes the dialog.
     */
    void on_pushButton_AddProduct_clicked();

private:
    /**
     * @brief Pointer to the generated user interface.
     */
    Ui::Dialog_FindProduct *ui;

    /**
     * @brief Model used to display search results.
     */
    QStandardItemModel *m_model;

    /**
     * @brief Cached list of products returned by the last search.
     */
    QList<Product> m_products;

    /**
     * @brief Populates the table view with the specified products.
     *
     * @param products List of products to display.
     */
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
