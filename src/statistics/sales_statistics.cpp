#include "sales_statistics.h"

#include "../database/database_Manager.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

/**
 * @brief Returns the singleton instance of SalesStatistics.
 *
 * Provides global access to the sales statistics manager while
 * ensuring that only one instance of the class exists.
 *
 * @return Reference to the SalesStatistics singleton instance.
 */
SalesStatistics& SalesStatistics::instance()
{
    static SalesStatistics instance;

    return instance;
}

/**
 * @brief Retrieves daily sales statistics for a date range.
 *
 * Queries the sales database and groups the results by sale date.
 * For each day, the total sales, gross profit, and net profit are
 * calculated using SQL aggregate functions.
 *
 * Only sales whose date falls within the specified range are included.
 *
 * @param startDate Start date of the statistics range.
 * @param endDate End date of the statistics range.
 *
 * @return List of DailySales containing the aggregated statistics
 *         for each day in the specified range. Returns an empty list
 *         if the database is unavailable or the query fails.
 */
QList<DailySales> SalesStatistics::getDailySales(
    const QDate& startDate,
    const QDate& endDate)
{
    QList<DailySales> dailySales;

    QSqlDatabase database =
        DatabaseManager::instance().getDatabase();

    /*
     * Verify that the database connection is available
     * before executing the statistics query.
     */
    if (!database.isOpen())
    {
        qDebug() << "Database is not open.";

        return dailySales;
    }

    QSqlQuery query(database);

    /*
     * Aggregate sales information by day.
     *
     * SUM(total) calculates the total sales.
     * SUM(gross_profit) calculates the total gross profit.
     * SUM(net_profit) calculates the total net profit.
     */
    query.prepare(
        "SELECT "
        "DATE(sale_date) AS sale_day, "
        "SUM(total) AS total_sales, "
        "SUM(gross_profit) AS total_gross_profit, "
        "SUM(net_profit) AS total_net_profit "
        "FROM sales "
        "WHERE DATE(sale_date) BETWEEN :startDate AND :endDate "
        "GROUP BY DATE(sale_date) "
        "ORDER BY DATE(sale_date)"
        );

    /*
     * Bind the requested date range to the SQL query.
     */
    query.bindValue(
        ":startDate",
        startDate.toString("yyyy-MM-dd"));

    query.bindValue(
        ":endDate",
        endDate.toString("yyyy-MM-dd"));

    /*
     * Execute the query and return an empty list if it fails.
     */
    if (!query.exec())
    {
        qDebug() << "Error getting daily sales:"
                 << query.lastError().text();

        return dailySales;
    }

    /*
     * Convert each database record into a DailySales structure.
     */
    while (query.next())
    {
        DailySales day;

        day.date =
            QDate::fromString(
                query.value("sale_day").toString(),
                "yyyy-MM-dd");

        day.total =
            query.value("total_sales").toDouble();

        day.grossProfit =
            query.value("total_gross_profit").toDouble();

        day.netProfit =
            query.value("total_net_profit").toDouble();

        dailySales.append(day);
    }

    return dailySales;
}