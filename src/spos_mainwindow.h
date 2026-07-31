/**
 * @file spos_mainwindow.h
 * @brief Declaration of the main window class for Smart Point Of Sale.
 *
 * Contains the definition of the main application window responsible for
 * managing product operations and the current sale process.
 */

#ifndef SPOS_MAINWINDOW_H
#define SPOS_MAINWINDOW_H

#include <QMainWindow>
#include "../src/models/Product.h"
#include "../src/sale/sale_mgr.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SPOS_MainWindow;
}
QT_END_NAMESPACE

/**
 * @class SPOS_MainWindow
 * @brief Main window controller for the Smart Point Of Sale application.
 *
 * Handles the main user interface, product management actions and barcode
 * scanning workflow. It allows adding, modifying, deleting and searching
 * products, as well as managing products in the current sale.
 */
class SPOS_MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        /**
         * @brief Creates the main application window.
         *
         * Initializes the UI and prepares the components required for product
         * management and sales operations.
         *
         * @param parent Parent widget.
         */
        explicit SPOS_MainWindow(QWidget *parent = nullptr);

        /**
         * @brief Destroys the main application window.
         *
         * Releases allocated UI resources.
         */
        ~SPOS_MainWindow() override;

    private:
        /**
         * @brief Current active sale instance.
         *
         * Stores the products, quantities and calculations associated with the
         * current transaction. The Sale object manages the sale data while the
         * main window is responsible for displaying it.
         */
        Sale m_sale;
        /**
         * @brief Pointer to the generated UI object.
         *
         * Provides access to all widgets created using Qt Designer.
         */
        Ui::SPOS_MainWindow *ui;

        /**
         * @brief Refreshes the sale products table.
         *
         * Displays the current items from the active sale in the products table.
         */
        void updateSaleTable();

        /**
         * @brief Updates the total amount of the current sale.
         *
         * Calculates the sum of all product subtotals displayed in the sale table
         * and updates the total field.
         */
        void updateSaleTotal();

        /**
         * @brief Resets the sale-related user interface.
         */
        void resetSaleUI();

    private slots:
        /**
         * @brief Opens the Add Product dialog.
         */
        void on_action_AddNewProduct_triggered();

        /**
         * @brief Opens the Modify Product dialog.
         */
        void on_action_ModifyProduct_triggered();

        /**
         * @brief Opens the Delete Product dialog.
         */
        void on_action_DeleteProduct_triggered();

        /**
         * @brief Opens the Find Product dialog.
         */
        void on_pushButton_FindProduct_clicked();

        /**
         * @brief Processes barcode input from the scanner.
         *
         * Searches the product database using the scanned barcode and adds the
         * product to the current sale.
         */
        void on_lineEdit_Barcode_returnPressed();

        /**
         * @brief Handles the received money input.
         *
         * Calculates the customer's change based on the received amount and the
         * current sale total.
         */
        void on_lineEdit_Received_returnPressed();

        /**
         * @brief Starts a new sale transaction.
         */
        void on_pushButton_NewSale_clicked();

        /**
         * @brief Cancels the current sale.
         */
        void on_pushButton_CancelSale_clicked();

        /**
         * @brief Removes the selected product from the current sale.
         */
        void on_pushButton_RemoveProduct_clicked();

        /**
         * @brief Handles changes made to sale items in the products table.
         *
         * Updates the current sale when the quantity or unit price of a product
         * is modified from the sale table. After updating the sale data, the row
         * subtotal and the total sale amount are recalculated.
         *
         * @param row Row index of the modified item.
         * @param column Column index of the modified cell.
         */
        void onSaleItemChanged(int row, int column);
};
#endif // SPOS_MAINWINDOW_H
