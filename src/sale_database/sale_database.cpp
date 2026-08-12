#include "sale_database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

/**
 * @brief Saves a completed sale and its items to the database.
 *
 * Generates the next ticket number automatically and stores
 * the sale information in the sales table. The products included
 * in the sale are then stored in the sale_items table.
 *
 * The operation is executed inside a database transaction.
 * If any operation fails, all changes are rolled back.
 *
 * @param sale Completed sale containing its products and total.
 *
 * @return true if the sale and all its items were successfully saved.
 * @return false if any database operation failed.
 */
bool SaleDatabase::saveSale(const CurrentSale& sale)
{
    QSqlDatabase database = QSqlDatabase::database();

    if(!database.transaction())
    {
        qDebug() << "Failed to start database transaction:"
                 << database.lastError().text();

        return false;
    }

    QSqlQuery query;

    double total = sale.getTotal();

    double grossProfit = 0.0;

    for(const SaleItem& item : sale.getItems())
    {
        grossProfit +=
            (item.product.sale_price - item.product.purchase_price)
            * item.quantity;
    }

    query.prepare(
        "SELECT COALESCE(MAX(ticket_number), 0) + 1 "
        "FROM sales"
        );

    if(!query.exec() || !query.next())
    {
        qDebug() << "Ticket number error:"
                 << query.lastError().text();

        database.rollback();
        return false;
    }

    int ticketNumber = query.value(0).toInt();

    query.prepare(
        "INSERT INTO sales "
        "(ticket_number, sale_date, total, gross_profit, net_profit) "
        "VALUES (?, ?, ?, ?, ?)"
        );

    query.addBindValue(ticketNumber);
    query.addBindValue(
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    query.addBindValue(total);
    query.addBindValue(grossProfit);
    query.addBindValue(0.0);

    if(!query.exec())
    {
        qDebug() << "Save sale error:"
                 << query.lastError().text();

        database.rollback();
        return false;
    }

    int saleId = query.lastInsertId().toInt();

    for(const SaleItem& item : sale.getItems())
    {
        double subtotal =
            item.quantity * item.product.sale_price;

        query.prepare(
            "INSERT INTO sale_items "
            "(sale_id, product_id, quantity, unit_price, subtotal) "
            "VALUES (?, ?, ?, ?, ?)"
            );

        query.addBindValue(saleId);
        query.addBindValue(item.product.id);
        query.addBindValue(item.quantity);
        query.addBindValue(item.product.sale_price);
        query.addBindValue(subtotal);

        if(!query.exec())
        {
            qDebug() << "Save sale item error:"
                     << query.lastError().text();

            database.rollback();
            return false;
        }
    }

    if(!database.commit())
    {
        qDebug() << "Failed to commit sale:"
                 << database.lastError().text();

        database.rollback();
        return false;
    }

    qDebug() << "Sale saved successfully."
             << "Ticket:"
             << QString("%1").arg(ticketNumber, 6, 10, QChar('0'))
             << "Total:"
             << total;

    return true;
}