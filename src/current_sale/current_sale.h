#ifndef CURRENT_SALE_H
#define CURRENT_SALE_H

#include "../models/Product.h"
#include <QList>

/**
 * @struct SaleItem
 * @brief Represents a product entry in a sale transaction.
 *
 * Stores the product information and the quantity selected by the customer
 * during the current sale.
 */
struct SaleItem
{
    /**
     * @brief Product information associated with this sale item.
     */
    Product product;

    /**
     * @brief Number of units of the product.
     */
    double quantity;
};

/**
 * @class CurrentSale
 * @brief Manages the current sale transaction.
 *
 * Handles the products added to the current sale, manages product quantities
 * and calculates the total amount of the transaction.
 *
 * The Sale class is independent from the user interface and only manages
 * the business logic of a sale.
 */
class CurrentSale
{
public:
    /**
     * @brief Adds a product to the current sale.
     *
     * If the product already exists in the sale, its quantity is increased.
     * Otherwise, a new sale item is created.
     *
     * @param product Product to add to the sale.
     */
    void addProduct(const Product& product);

    /**
     * @brief Gets all products included in the current sale.
     *
     * @return List of sale items containing products and quantities.
     */
    QList<SaleItem> getItems() const;

    /**
     * @brief Calculates the total amount of the current sale.
     *
     * Calculates the sum of all products using their sale price and quantity.
     *
     * @return Total sale amount.
     */
    double getTotal() const;

    /**
     * @brief Clears all products from the current sale.
     *
     * Removes all sale items and resets the current transaction.
     */
    void clear();

    /**
     * @brief Removes a product from the current sale.
     *
     * Removes the sale item associated with the specified product ID.
     *
     * @param productId Unique identifier of the product to remove.
     */
    void removeProduct(int productId);

    /**
     * @brief Updates the quantity of a product in the current sale.
     *
     * @param productId Product identifier.
     * @param quantity New quantity.
     * @return true if the product was found and updated.
     */
    bool updateQuantity(int productId, double quantity);

    /**
     * @brief Updates the unit sale price of a product.
     *
     * @param productId Product identifier.
     * @param price New unit price.
     * @return true if the product was found and updated.
     */
    bool updateUnitPrice(int productId, double price);

private:
    /**
     * @brief List of products currently included in the sale.
     */
    QList<SaleItem> m_items;
};

#endif // CURRENT_SALE_H