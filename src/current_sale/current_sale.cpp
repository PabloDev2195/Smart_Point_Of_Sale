#include "current_sale.h"

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
void CurrentSale::addProduct(const Product& product)
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
QList<SaleItem> CurrentSale::getItems() const
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
double CurrentSale::getTotal() const
{
    double total = 0.0;

    for(const SaleItem &item : m_items)
    {
        total += item.quantity * item.product.sale_price;
    }

    return total;
}

/**
 * @brief Clears all products from the current sale.
 *
 * Removes all items stored in the current transaction.
 */
void CurrentSale::clear()
{
    m_items.clear();
}

/**
 * @brief Removes a product from the current sale.
 *
 * Searches the current sale for the specified product ID and removes the
 * corresponding sale item if it exists.
 *
 * @param productId Unique identifier of the product to remove.
 */
void CurrentSale::removeProduct(int productId)
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items[i].product.id == productId)
        {
            m_items.removeAt(i);
            return;
        }
    }
}

/**
 * @brief Updates the quantity of a product in the current sale.
 *
 * Searches for a product in the current sale items using its unique product ID
 * and updates its quantity with the provided value.
 *
 * The quantity must be greater than zero. If the product is found and the
 * quantity is valid, the item is updated and the function returns true.
 *
 * @param productId Unique identifier of the product to update.
 * @param quantity New quantity value for the product.
 *
 * @return true If the product quantity was successfully updated.
 * @return false If the quantity is invalid or the product was not found.
 */
bool CurrentSale::updateQuantity(int productId, double quantity)
{
    if (quantity <= 0)
        return false;

    for (SaleItem &item : m_items)
    {
        if (item.product.id == productId)
        {
            item.quantity = quantity;
            return true;
        }
    }

    return false;
}

/**
 * @brief Updates the unit price of a product in the current sale.
 *
 * Searches for a product in the current sale items using its unique product ID
 * and updates the sale price with the provided value.
 *
 * The price cannot be negative. If the product is found and the price is valid,
 * the item price is updated and the function returns true.
 *
 * @param productId Unique identifier of the product to update.
 * @param price New unit price value for the product.
 *
 * @return true If the product price was successfully updated.
 * @return false If the price is invalid or the product was not found.
 */
bool CurrentSale::updateUnitPrice(int productId, double price)
{
    if (price < 0.0)
        return false;

    for (SaleItem &item : m_items)
    {
        if (item.product.id == productId)
        {
            item.product.sale_price = price;
            return true;
        }
    }

    return false;
}
