#include "dialog_payment.h"
#include "ui_dialog_payment.h"

/**
 * @brief Constructs a payment dialog.
 *
 * Initializes the payment window with the total amount of the current sale.
 * The dialog displays the amount to be paid and allows the user to enter
 * the customer's payment amount.
 *
 * @param total Total amount of the current transaction.
 * @param parent Parent widget that owns this dialog.
 */
Dialog_Payment::Dialog_Payment(double total, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_Payment)
    , m_total(total)
    , m_payment(0.0)
    , m_change(0.0)
{
    ui->setupUi(this);

    ui->label_TotalNum->setText(
        QString("$ %1").arg(m_total, 0, 'f', 2)
        );

     ui->lineEdit_Payment->setFocus();
}

/**
 * @brief Destructor of Dialog_Payment.
 *
 * Releases the memory allocated for the UI object.
 */
Dialog_Payment::~Dialog_Payment()
{
    delete ui;
}

/**
 * @brief Handles the confirmation button click event.
 *
 * Retrieves the payment amount entered by the customer,
 * calculates the change to return, and validates if the
 * received amount is enough to complete the transaction.
 *
 * If the payment is greater than or equal to the total,
 * the dialog is accepted. Otherwise, an error message
 * is displayed.
 */
void Dialog_Payment::on_pushButton_Confirm_clicked()
{
    m_payment = ui->lineEdit_Payment->text().toDouble();

    m_change = m_payment - m_total;

    if (m_change >= 0)
    {
        ui->label_ChangeNum->setText(
            QString("$ %1").arg(m_change, 0, 'f', 2)
            );
        accept();
    }
    else
    {
        ui->label_ChangeNum->setText("Falta dinero");
    }
}

/**
 * @brief Returns the amount received from the customer.
 *
 * This value is captured from the payment input field.
 *
 * @return Customer payment amount.
 */
double Dialog_Payment::getPayment() const
{
    return m_payment;
}

/**
 * @brief Returns the calculated change.
 *
 * The change is calculated as:
 *
 * Change = Payment - Total
 *
 * @return Amount of money to return to the customer.
 */
double Dialog_Payment::getChange() const
{
    return m_change;
}
