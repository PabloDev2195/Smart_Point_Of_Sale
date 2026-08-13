#include "dialog_ticket.h"
#include "ui_dialog_ticket.h"
#include "../sale_database/sale_database.h"

/**
 * @brief Constructs the ticket details dialog.
 *
 * Initializes the ticket details interface and retrieves the
 * information associated with the specified ticket number.
 *
 * The dialog displays the ticket date and total amount, as well
 * as the products included in the ticket with their quantities
 * and subtotals.
 *
 * @param ticketNumber Ticket number whose details will be displayed.
 * @param parent Parent widget of the dialog.
 */
Dialog_Ticket::Dialog_Ticket(int ticketNumber, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_Ticket)
    , m_ticketNumber(ticketNumber)
{
    ui->setupUi(this);

    ui->tableWidget_Products->setColumnCount(3);

    ui->tableWidget_Products->setHorizontalHeaderLabels({
        "Producto",
        "Cantidad",
        "Subtotal"
    });

    ui->tableWidget_Products->setColumnWidth(0, 350);

    ui->tableWidget_Products->horizontalHeader()->setSectionResizeMode(
        2, QHeaderView::Stretch);

    SaleDatabase saleDatabase;

    /*
     * Retrieve general ticket information from the database.
     */
    const SaleHistory sale =
        saleDatabase.getSaleDetails(m_ticketNumber);

    ui->label_ID->setText(
        QString("%1")
            .arg(sale.ticketNumber,
                 6,
                 10,
                 QChar('0')));

    ui->label_Date->setText(
        sale.saleDate);

    ui->label_Total->setText(
        QString("$ %1")
            .arg(sale.total, 0, 'f', 2));

    /*
     * Retrieve the products associated with the ticket.
     */
    const QList<SaleItemHistory> items =
        saleDatabase.getSaleItems(m_ticketNumber);

    ui->tableWidget_Products->setRowCount(0);

    for (const SaleItemHistory& item : items)
    {
        const int row =
            ui->tableWidget_Products->rowCount();

        ui->tableWidget_Products->insertRow(row);

        ui->tableWidget_Products->setItem(
            row,
            0,
            new QTableWidgetItem(
                item.productName));

        ui->tableWidget_Products->setItem(
            row,
            1,
            new QTableWidgetItem(
                QString::number(item.quantity)));

        ui->tableWidget_Products->setItem(
            row,
            2,
            new QTableWidgetItem(
                QString("$ %1")
                    .arg(item.subtotal, 0, 'f', 2)));

        ui->tableWidget_Products->item(row, 1)
            ->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget_Products->item(row, 2)
            ->setTextAlignment(Qt::AlignCenter);
    }
}

/**
 * @brief Destroys the ticket details dialog.
 *
 * Releases the memory allocated for the user interface.
 */
Dialog_Ticket::~Dialog_Ticket()
{
    delete ui;
}