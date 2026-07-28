/**
 * @file product_mgr.cpp
 * @brief Implementation of the ProductManager class.
 *
 * This file contains the implementation of the methods responsible for
 * managing product-related operations in the SQLite database.
 */

#include "product_mgr.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

/**
 * @brief Returns the singleton instance of ProductManager.
 *
 * Implements the Singleton design pattern to ensure that only one
 * ProductManager instance exists throughout the application's lifetime.
 *
 * @return Reference to the unique ProductManager instance.
 */
ProductManager& ProductManager::instance()
{
    static ProductManager instance;
    return instance;
}

/**
 * @brief Adds a new product to the database.
 *
 * Inserts a new record into the `products` table using a prepared
 * SQL statement. Prepared statements improve security by preventing
 * SQL injection attacks and efficiently binding parameter values.
 *
 * The following product information is stored:
 * - Product name
 * - Barcode
 * - Purchase price
 * - Sale price
 *
 * @param product A Product object containing the information to be
 * inserted into the database.
 *
 * @return true if the product was successfully inserted.
 * @return false if the SQL query execution failed.
 */
bool ProductManager::addProduct(const Product& product)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO products "
        "(name, barcode, purchase_price, sale_price) "
        "VALUES (?, ?, ?, ?)");

    query.addBindValue(product.name);
    query.addBindValue(product.barcode);
    query.addBindValue(product.purchase_price);
    query.addBindValue(product.sale_price);

    return query.exec();
}