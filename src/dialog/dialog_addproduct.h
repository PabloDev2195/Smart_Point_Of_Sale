#ifndef DIALOG_ADDPRODUCT_H
#define DIALOG_ADDPRODUCT_H

#include <QDialog>

namespace Ui {
    /**
     * @brief Forward declaration of the generated UI class.
     */
    class Dialog_AddProduct;
}

/**
 * @brief Dialog used to create a new product.
 *
 * This dialog allows the user to:
 * - Enter product information.
 * - Validate the required fields.
 * - Save the product to the database.
 * - Cancel the operation without saving.
 */
class Dialog_AddProduct : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs the Add Product dialog.
     *
     * @param parent Parent widget.
     */
    explicit Dialog_AddProduct(QWidget *parent = nullptr);

    /**
     * @brief Destroys the dialog.
     */
    ~Dialog_AddProduct();

private slots:
    /**
     * @brief Validates the entered information and saves the product.
     */
    void on_pushButton_Save_clicked();

    /**
     * @brief Cancels the product creation and closes the dialog.
     */
    void on_pushButton_Cancel_clicked();

private:
    /**
     * @brief Pointer to the generated user interface.
     */
    Ui::Dialog_AddProduct *ui;
};

#endif // DIALOG_ADDPRODUCT_H
