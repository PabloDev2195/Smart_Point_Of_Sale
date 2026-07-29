#include "Sale_mgr.h"

/**
 * @brief Adds a product to the current sale.
 *
 * Checks if the product already exists in the current sale using its barcode.
 * If the product is already present, its quantity is increased. Otherwise,
 * a new sale item is created and added to the sale list with an initial
 * quantity of one.
 *
 * @param product Product to be added to the sale.
 */
void Sale::addProduct(const Product& product)
{
    for (SaleItem &item : m_items)
    {
        if(item.product.barcode == product.barcode)
        {
            item.quantity++;
            return;
        }
    }

    SaleItem newItem;

    newItem.product = product;
    newItem.quantity = 1;

    m_items.append(newItem);
}

/**
 * @brief Returns the items included in the current sale.
 *
 * Provides access to the list of products and their quantities currently
 * stored in the active sale.
 *
 * @return QList<SaleItem> List of sale items.
 */
QList<SaleItem> Sale::getItems() const
{
    return m_items;
}

/**
 * @brief Calculates the total amount of the current sale.
 *
 * Iterates through all sale items and calculates the total amount by
 * multiplying each product sale price by its quantity.
 *
 * @return double Total amount of the current sale.
 */
double Sale::getTotal() const
{
    double total = 0.0;

    for(const SaleItem &item : m_items)
    {
        total += item.quantity * item.product.sale_price;
    }

    return total;
}