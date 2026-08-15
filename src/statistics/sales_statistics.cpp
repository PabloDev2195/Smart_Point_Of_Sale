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

/**
 * @brief Retrieves product sales statistics for a date range.
 *
 * Calculates the total quantity sold for each product within
 * the specified date range.
 *
 * @param startDate Start date of the requested range.
 * @param endDate End date of the requested range.
 *
 * @return List of product sales statistics containing the product
 *         name and total quantity sold.
 */
QList<ProductSales> SalesStatistics::getProductSales(
    const QDate& startDate,
    const QDate& endDate)
{
    QList<ProductSales> productSales;

    QSqlDatabase database =
        DatabaseManager::instance().getDatabase();

    if (!database.isOpen())
    {
        qDebug() << "Database is not open.";

        return productSales;
    }

    QSqlQuery query(database);

    query.prepare(
        "SELECT "
        "p.name AS product_name, "
        "SUM(si.quantity) AS total_quantity "
        "FROM sale_items si "
        "INNER JOIN sales s "
        "ON si.sale_id = s.id "
        "INNER JOIN products p "
        "ON si.product_id = p.id "
        "WHERE DATE(s.sale_date) BETWEEN :startDate AND :endDate "
        "GROUP BY si.product_id "
        "ORDER BY total_quantity DESC"
        );

    query.bindValue(
        ":startDate",
        startDate.toString("yyyy-MM-dd"));

    query.bindValue(
        ":endDate",
        endDate.toString("yyyy-MM-dd"));

    if (!query.exec())
    {
        qDebug() << "Error getting product sales:"
                 << query.lastError().text();

        return productSales;
    }

    while (query.next())
    {
        ProductSales product;

        product.productName =
            query.value("product_name").toString();

        product.quantity =
            query.value("total_quantity").toDouble();

        productSales.append(product);
    }

    return productSales;
}