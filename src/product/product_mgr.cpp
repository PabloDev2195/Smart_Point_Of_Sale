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

/**
 * @brief Searches for products by barcode, name, or both.
 *
 * Executes a parameterized SQL query against the `products` table
 * using optional search criteria. If a barcode or name is provided,
 * the corresponding filter is added to the query using the SQL
 * `LIKE` operator, allowing partial matches.
 *
 * Matching records are converted into Product objects and returned
 * as a list.
 *
 * @param barcode Barcode used as a search criterion. If empty,
 *                no barcode filter is applied.
 * @param name Product name used as a search criterion. If empty,
 *             no name filter is applied.
 *
 * @return A QList containing all products that match the specified
 * search criteria. Returns an empty list if no products are found
 * or if the query execution fails.
 */
QList<Product> ProductManager::findProducts(const QString& barcode,
                                            const QString& name)
{
    QList<Product> products;

    QSqlQuery query;

    QString sql =
        "SELECT id, name, barcode, purchase_price, sale_price "
        "FROM products "
        "WHERE 1 = 1 ";

    if (!barcode.isEmpty())
        sql += "AND barcode LIKE ? ";

    if (!name.isEmpty())
        sql += "AND name LIKE ? ";

    query.prepare(sql);

    if (!barcode.isEmpty())
        query.addBindValue("%" + barcode + "%");

    if (!name.isEmpty())
        query.addBindValue("%" + name + "%");

    if(query.exec())
    {
        while(query.next())
        {
            Product product;

            product.id = query.value(0).toInt();
            product.name = query.value(1).toString();
            product.barcode = query.value(2).toString();
            product.purchase_price = query.value(3).toDouble();
            product.sale_price = query.value(4).toDouble();

            products.append(product);
        }
    }

    return products;
}