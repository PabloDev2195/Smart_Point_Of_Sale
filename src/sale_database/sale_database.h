#ifndef SALE_DATABASE_H
#define SALE_DATABASE_H

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
     * @brief Saves a completed sale to the database.
     *
     * Generates the next ticket number automatically and stores
     * the sale date, total amount, gross profit, and net profit.
     *
     * Gross and net profit are initialized to zero and will be
     * calculated in later functionality.
     *
     * @param total Total amount of the completed sale.
     *
     * @return true if the sale was successfully saved.
     * @return false if the database operation failed.
     */
    bool saveSale(double total);
};

#endif // SALE_DATABASE_H