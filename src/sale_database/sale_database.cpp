#include "sale_database.h"

#include "../database/database_Manager.h"

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

/**
 * @brief Returns the last ticket number stored in the database.
 *
 * Queries the sales table and retrieves the highest ticket number
 * currently stored. If no sales exist, the function returns 0.
 *
 * The database connection is obtained from DatabaseManager.
 *
 * @return The last ticket number stored in the database,
 *         or 0 if no tickets exist or an error occurs.
 */
int SaleDatabase::getLastTicketNumber()
{
    QSqlQuery query(DatabaseManager::instance().getDatabase());

    if (!query.exec(
            "SELECT COALESCE(MAX(ticket_number), 0) "
            "FROM sales"))
    {
        qDebug() << "Error getting last ticket:"
                 << query.lastError().text();

        return 0;
    }

    if (query.next())
    {
        return query.value(0).toInt() + 1;
    }

    return 0;
}

/**
 * @brief Retrieves sales history for a specific date range.
 *
 * Queries the sales table and returns all sales whose sale date
 * falls between the specified start and end dates.
 *
 * The results are ordered by ticket number in descending order,
 * with the most recent ticket displayed first.
 *
 * @param startDate Start date of the search range.
 * @param endDate End date of the search range.
 *
 * @return A list containing the sales found within the specified
 *         date range. Returns an empty list if the query fails
 *         or no sales match the specified dates.
 */
QList<SaleHistory> SaleDatabase::getSalesHistory(
    const QDate& startDate,
    const QDate& endDate)
{
    QList<SaleHistory> salesHistory;

    QSqlQuery query(DatabaseManager::instance().getDatabase());

    query.prepare(
        "SELECT ticket_number, "
        "sale_date, "
        "total, "
        "gross_profit, "
        "net_profit "
        "FROM sales "
        "WHERE DATE(sale_date) BETWEEN :startDate AND :endDate "
        "ORDER BY ticket_number DESC");

    query.bindValue(":startDate",
                    startDate.toString("yyyy-MM-dd"));

    query.bindValue(":endDate",
                    endDate.toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug() << "Error getting sales history:"
                 << query.lastError().text();

        return salesHistory;
    }

    while (query.next())
    {
        SaleHistory sale;

        sale.ticketNumber =
            query.value("ticket_number").toInt();

        sale.saleDate =
            query.value("sale_date").toString();

        sale.total =
            query.value("total").toDouble();

        sale.grossProfit =
            query.value("gross_profit").toDouble();

        sale.netProfit =
            query.value("net_profit").toDouble();

        salesHistory.append(sale);
    }

    return salesHistory;
}

/**
 * @brief Retrieves all sales history from the database.
 *
 * Queries the sales table and returns all stored sales without
 * applying a date filter.
 *
 * The results are ordered by ticket number in descending order,
 * with the most recent ticket displayed first.
 *
 * @return A list containing all stored sales. Returns an empty
 *         list if the query fails or no sales exist.
 */
QList<SaleHistory> SaleDatabase::getAllSalesHistory()
{
    QList<SaleHistory> salesHistory;

    QSqlQuery query(DatabaseManager::instance().getDatabase());

    const QString sql =
        "SELECT ticket_number, "
        "sale_date, "
        "total, "
        "gross_profit, "
        "net_profit "
        "FROM sales "
        "ORDER BY ticket_number DESC";

    if (!query.exec(sql))
    {
        qDebug() << "Error getting all sales history:"
                 << query.lastError().text();

        return salesHistory;
    }

    while (query.next())
    {
        SaleHistory sale;

        sale.ticketNumber =
            query.value("ticket_number").toInt();

        sale.saleDate =
            query.value("sale_date").toString();

        sale.total =
            query.value("total").toDouble();

        sale.grossProfit =
            query.value("gross_profit").toDouble();

        sale.netProfit =
            query.value("net_profit").toDouble();

        salesHistory.append(sale);
    }

    return salesHistory;
}