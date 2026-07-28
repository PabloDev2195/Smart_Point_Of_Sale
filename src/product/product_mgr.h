/**
 * @file product_mgr.h
 * @brief Declaration of the ProductManager class.
 *
 * This class provides an interface for managing product records
 * in the database. It implements the Singleton design pattern
 * to ensure a single access point for all product-related
 * database operations.
 */

#ifndef PRODUCT_MGR_H
#define PRODUCT_MGR_H

#include "../models/Product.h"

/**
 * @class ProductManager
 * @brief Manages CRUD operations for products.
 *
 * The ProductManager class is responsible for creating, reading,
 * updating, and deleting product records in the database.
 * It exposes a singleton instance to centralize database access.
 */
class ProductManager
{
public:
    /**
     * @brief Returns the singleton instance of ProductManager.
     *
     * @return Reference to the unique ProductManager instance.
     */
    static ProductManager& instance();

    /**
     * @brief Inserts a new product into the database.
     *
     * @param product Product object containing the information to insert.
     * @return true if the product was successfully inserted.
     * @return false otherwise.
     */
    bool addProduct(const Product& product);

    /**
     * @brief Updates an existing product in the database.
     *
     * @param product Product object containing the updated information.
     * @return true if the product was successfully updated.
     * @return false otherwise.
     */
    bool updateProduct(const Product& product);

    /**
     * @brief Deletes a product from the database.
     *
     * @param id Unique identifier of the product.
     * @return true if the product was successfully deleted.
     * @return false otherwise.
     */
    bool deleteProduct(int id);

    /**
     * @brief Retrieves all products from the database.
     *
     * @return A list containing all stored products.
     */
    QList<Product> getProducts();

    /**
     * @brief Retrieves a product by its identifier.
     *
     * @param id Unique identifier of the product.
     * @return Product object corresponding to the specified identifier.
     */
    Product getProduct(int id);

    /**
     * @brief Retrieves products matching the given filters.
     *
     * Searches the products table using optional filters. The method allows
     * searching by barcode, product name, or both.
     *
     * @param barcode Product barcode filter.
     * @param name Product name filter.
     *
     * @return A list of matching Product objects. Returns an empty list if no
     *         products are found or the query fails.
     */
    QList<Product> findProducts(const QString& barcode,
                                const QString& name);

private:
    /**
     * @brief Default constructor.
     *
     * Declared private to enforce the Singleton design pattern.
     */
    ProductManager() = default;
};

#endif //PRODUCT_MGR_H