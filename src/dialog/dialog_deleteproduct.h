#ifndef DIALOG_DELETEPRODUCT_H
#define DIALOG_DELETEPRODUCT_H

#include <QDialog>

namespace Ui {
/**
 * @brief Forward declaration of the generated UI class.
 */
class Dialog_DeleteProduct;
}

/**
 * @brief Dialog used to search for and delete products.
 *
 * This dialog allows the user to:
 * - Search products by barcode or name.
 * - Display matching products in a table.
 * - Select a product for deletion.
 * - Delete the selected product after confirmation.
 */
class Dialog_DeleteProduct : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the Delete Product dialog.
     *
     * @param parent Parent widget.
     */
    explicit Dialog_DeleteProduct(QWidget *parent = nullptr);

    /**
     * @brief Destroys the dialog.
     */
    ~Dialog_DeleteProduct();

private slots:
    /**
     * @brief Searches for products matching the entered criteria.
     */
    void on_pushButton_Find_clicked();

    /**
     * @brief Stores the identifier of the selected product.
     *
     * @param row Selected table row.
     * @param column Selected table column.
     */
    void on_tableWidget_Products_cellClicked(int row, int column);

    /**
     * @brief Deletes the selected product.
     */
    void on_pushButton_Delete_clicked();

    /**
     * @brief Cancels the delete operation and closes the dialog.
     */
    void on_pushButton_Cancel_clicked();

private:
    /**
     * @brief Pointer to the generated user interface.
     */
    Ui::Dialog_DeleteProduct *ui;

    /**
     * @brief Identifier of the currently selected product.
     *
     * A value of -1 indicates that no product has been selected.
     */
    int m_selectedProductId = -1;
};

#endif // DIALOG_DELETEPRODUCT_H
