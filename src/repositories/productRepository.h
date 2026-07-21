#ifndef PRODUCT_REPOSITORY_H
#define PRODUCT_REPOSITORY_H

#include "../models/Product.h"
#include <QList>

/**
 * @class ProductRepository
 *
 * @brief Provides database operations for product entities.
 *
 * This class acts as a repository layer between the application
 * logic and the database. It provides CRUD operations to manage
 * product records stored in the SQLite database.
 */
class ProductRepository
{
public:

    /**
     * @brief Constructs a ProductRepository object.
     */
    ProductRepository();


    /**
     * @brief Adds a new product to the database.
     *
     * Creates a new product record using the information
     * provided by the Product object.
     *
     * @param product Product object containing the data to insert.
     *
     * @return true if the product was inserted successfully.
     * @return false if the insert operation failed.
     */
    bool addProduct(const Product& product);


    /**
     * @brief Retrieves all products from the database.
     *
     * Reads all product records from the database and returns
     * them as a list of Product objects.
     *
     * @return QList<Product> List containing all stored products.
     */
    QList<Product> getProducts();


    /**
     * @brief Updates an existing product.
     *
     * Updates the product information identified by its unique ID.
     *
     * @param product Product object containing the updated information.
     *
     * @return true if the product was updated successfully.
     * @return false if the update operation failed or the product
     * was not found.
     */
    bool updateProduct(const Product& product);


    /**
     * @brief Deletes a product from the database.
     *
     * Removes a product record using its unique identifier.
     *
     * @param id Product identifier used to remove the record.
     *
     * @return true if the product was deleted successfully.
     * @return false if the delete operation failed or the product
     * was not found.
     */
    bool deleteProduct(int id);

};

#endif // PRODUCT_REPOSITORY_H