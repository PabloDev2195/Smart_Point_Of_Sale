#ifndef DIALOG_MODIFYPRODUCT_H
#define DIALOG_MODIFYPRODUCT_H

#include <QDialog>
#include "../models/Product.h"

namespace Ui {
/**
 * @brief Forward declaration of the generated UI class.
 */
class Dialog_ModifyProduct;
}

/**
 * @brief Dialog used to search for and modify existing products.
 *
 * This dialog allows the user to:
 * - Search products by barcode or name.
 * - Select a product from the search results.
 * - Edit the product information.
 * - Save the updated information to the database.
 */
class Dialog_ModifyProduct : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the Modify Product dialog.
     *
     * @param parent Parent widget.
     */
    explicit Dialog_ModifyProduct(QWidget *parent = nullptr);

    /**
     * @brief Destroys the dialog.
     */
    ~Dialog_ModifyProduct();

private:
    /**
     * @brief Pointer to the generated user interface.
     */
    Ui::Dialog_ModifyProduct *ui;

    /**
     * @brief Currently selected product being modified.
     */
    Product m_product;

private slots:
    /**
     * @brief Loads the selected product into the editing controls.
     *
     * @param row Selected table row.
     * @param column Selected table column.
     */
    void loadSelectedProduct(int row, int column);

    /**
     * @brief Searches for products matching the entered criteria.
     */
    void on_pushButton_Find_clicked();

    /**
     * @brief Saves the modified product information.
     */
    void on_pushButton_Save_clicked();

    /**
     * @brief Cancels the modification and closes the dialog.
     */
    void on_pushButton_Cancel_clicked();
};

#endif // DIALOG_MODIFYPRODUCT_H
