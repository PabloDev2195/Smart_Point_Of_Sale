#ifndef SALES_MGR_H
#define SALES_MGR_H

#include <QList>
#include <QString>

/**
 * @brief Represents a completed sale stored in the database.
 *
 * Contains the summary information of a completed sale.
 */
struct SaleRecord
{
    int id = 0;
    int ticketNumber = 0;
    QString saleDate;
    double total = 0.0;
    double grossProfit = 0.0;
    double netProfit = 0.0;
};

/**
 * @brief Represents an item from a completed sale stored in the database.
 *
 * Contains the product information and sale-specific values
 * recorded when the transaction was completed.
 */
struct SaleItemRecord
{
    int productId = 0;
    QString productName;
    QString barcode;
    double quantity = 0.0;
    double unitPrice = 0.0;
    double subtotal = 0.0;
};

/**
 * @brief Manages queries for completed sales.
 *
 * Provides access to historical sales stored in the database.
 */
class SalesMgr
{
public:

    /**
     * @brief Retrieves all completed sales.
     *
     * Sales are returned ordered by ticket number
     * from the most recent to the oldest.
     *
     * @return QList<SaleRecord> List of completed sales.
     */
    QList<SaleRecord> getSales();

    /**
     * @brief Retrieves a completed sale by ticket number.
     *
     * @param ticketNumber Ticket number of the sale.
     *
     * @return SaleRecord Sale information.
     * @return Empty SaleRecord if the sale is not found.
     */
    SaleRecord getSale(int ticketNumber);

    /**
     * @brief Retrieves all items belonging to a completed sale.
     *
     * @param saleId Database ID of the completed sale.
     *
     * @return QList<SaleItemRecord> List of sale items.
     */
    QList<SaleItemRecord> getSaleItems(int saleId);
};

#endif // SALES_MGR_H