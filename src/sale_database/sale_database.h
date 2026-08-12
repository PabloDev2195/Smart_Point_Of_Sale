#ifndef SALE_DATABASE_H
#define SALE_DATABASE_H

#include "../current_sale/current_sale.h"
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
};

#endif // SALE_DATABASE_H