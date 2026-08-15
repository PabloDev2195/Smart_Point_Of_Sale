#ifndef SALES_STATISTICS_H
#define SALES_STATISTICS_H

#include <QDate>
#include <QList>

/**
 * @brief Stores aggregated sales statistics for a specific day.
 *
 * Contains the total sales amount, gross profit, and net profit
 * calculated for a given date.
 */
struct DailySales
{
    /**
     * @brief Date associated with the sales statistics.
     */
    QDate date;

    /**
     * @brief Total sales amount for the day.
     */
    double total;

    /**
     * @brief Total gross profit for the day.
     */
    double grossProfit;

    /**
     * @brief Total net profit for the day.
     */
    double netProfit;
};

/**
 * @brief Stores aggregated sales information for a product.
 *
 * Contains the product name and the total quantity sold
 * within a specified date range.
 */
struct ProductSales
{
    /**
     * @brief Name of the product.
     */
    QString productName;

    /**
     * @brief Total quantity sold.
     */
    double quantity;
};

/**
 * @brief Provides sales statistics and aggregation functions.
 *
 * SalesStatistics uses a singleton pattern to provide access to
 * sales statistics retrieved from the database.
 */
class SalesStatistics
{
public:

    /**
     * @brief Returns the singleton instance.
     *
     * @return Reference to the SalesStatistics singleton instance.
     */
    static SalesStatistics& instance();

    /**
     * @brief Retrieves aggregated sales statistics by day.
     *
     * Returns sales totals, gross profits, and net profits grouped
     * by date within the specified date range.
     *
     * @param startDate Start date of the requested range.
     * @param endDate End date of the requested range.
     *
     * @return List of daily sales statistics.
     */
    QList<DailySales> getDailySales(
        const QDate& startDate,
        const QDate& endDate);

    /**
     * @brief Retrieves product sales statistics for a specified date range.
     *
     * Returns the total quantity sold for each product within the
     * specified date range.
     *
     * @param startDate Start date of the requested range.
     * @param endDate End date of the requested range.
     *
     * @return List of ProductSales containing the product name and
     *         total quantity sold.
     */
    QList<ProductSales> getProductSales(
        const QDate& startDate,
        const QDate& endDate);

private:

    /**
     * @brief Constructs a SalesStatistics instance.
     */
    SalesStatistics() = default;

    /**
     * @brief Destroys the SalesStatistics instance.
     */
    ~SalesStatistics() = default;

    /**
     * @brief Deleted copy constructor.
     *
     * Prevents copying of the singleton instance.
     */
    SalesStatistics(const SalesStatistics&) = delete;

    /**
     * @brief Deleted assignment operator.
     *
     * Prevents assignment of the singleton instance.
     */
    SalesStatistics& operator=(const SalesStatistics&) = delete;
};

#endif // SALES_STATISTICS_H