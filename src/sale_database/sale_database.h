#ifndef SALE_DATABASE_H
#define SALE_DATABASE_H

#include "../current_sale/current_sale.h"
#include <QDate>

/**
 * @brief Represents a sale record retrieved from the database.
 *
 * This structure contains the information required to display
 * a sale in the ticket history.
 */
struct SaleHistory
{
    int ticketNumber;
    QString saleDate;
    double total;
    double grossProfit;
    double netProfit;
};

/**
 * @brief Provides database operations for sales.
 *
 * This class is responsible for storing and retrieving
 * sales information from the SQLite database.
 */
class SaleDatabase
{
public:

    /**
     * @brief Saves a completed sale and its items to the database.
     *
     * @param sale Completed sale to be stored.
     *
     * @return true if the sale was successfully saved.
     * @return false if the database operation failed.
     */
    bool saveSale(const CurrentSale& sale);

    /**
     * @brief Returns the last ticket number stored in the database.
     *
     * Retrieves the highest ticket number currently stored in the
     * sales table. If no sales exist, the function returns 0.
     *
     * @return The last ticket number stored in the database,
     *         or 0 if no tickets exist.
     */
    int getLastTicketNumber();

    /**
     * @brief Retrieves sales history for a specific date range.
     *
     * Returns all sales stored in the database whose sale date
     * falls within the specified date range.
     *
     * @param startDate Start date of the search range.
     * @param endDate End date of the search range.
     *
     * @return A list containing the sales history within the
     *         specified date range.
     */
    QList<SaleHistory> getSalesHistory(
        const QDate& startDate,
        const QDate& endDate);

    /**
     * @brief Retrieves all sales history from the database.
     *
     * Returns all stored sales ordered by ticket number in
     * descending order.
     *
     * @return A list containing all stored sales.
     */
    QList<SaleHistory> getAllSalesHistory();
};

#endif // SALE_DATABASE_H