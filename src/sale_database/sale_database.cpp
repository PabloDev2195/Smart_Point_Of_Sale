#include "sale_database.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>

/**
 * @brief Saves a completed sale to the database.
 *
 * Generates the next ticket number automatically and stores
 * the sale date, total amount, gross profit, and net profit.
 *
 * @param total Total amount of the completed sale.
 *
 * @return true if the sale was successfully saved.
 * @return false if the database operation failed.
 */
bool SaleDatabase::saveSale(double total)
{
    QSqlQuery query;

    query.prepare(
        "SELECT COALESCE(MAX(ticket_number), 0) + 1 "
        "FROM sales"
    );

    if(!query.exec() || !query.next())
    {
        qDebug() << "Ticket number error:"
                 << query.lastError().text();

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
    query.addBindValue(0.0);
    query.addBindValue(0.0);

    if(!query.exec())
    {
        qDebug() << "Save sale error:"
                 << query.lastError().text();

        return false;
    }

    qDebug() << "Sale saved successfully."
             << "Ticket:"
             << QString("%1").arg(ticketNumber, 6, 10, QChar('0'))
             << "Total:"
             << total;

    return true;
}