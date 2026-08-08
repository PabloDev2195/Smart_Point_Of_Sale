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
#include <QDateTime>

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

/**
 * @brief Actualiza la información de un producto existente en la base de datos.
 *
 * Busca el producto mediante su identificador único (`id`) y actualiza
 * los campos nombre, código de barras, precio de compra y precio de venta.
 *
 * La consulta utiliza parámetros enlazados mediante `addBindValue()`,
 * asignando cada valor a los signos `?` en el mismo orden en que aparecen
 * dentro de la consulta SQL.
 *
 * @param product Objeto Product que contiene el ID del producto y los
 * nuevos datos que se guardarán en la base de datos.
 *
 * @return true si la consulta SQL se ejecutó correctamente.
 * @return false si ocurrió un error durante la actualización.
 */
bool ProductManager::updateProduct(const Product& product)
{
    QSqlQuery query;

    query.prepare(
        "UPDATE products "
        "SET name = ?, "
        "barcode = ?, "
        "purchase_price = ?, "
        "sale_price = ? "
        "WHERE id = ?"
        );

    query.addBindValue(product.name);
    query.addBindValue(product.barcode);
    query.addBindValue(product.purchase_price);
    query.addBindValue(product.sale_price);
    query.addBindValue(product.id);

    if (!query.exec())
    {
        qDebug() << "Update product failed:"
                 << query.lastError().text();

        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        qDebug() << "Product not found. ID:"
                 << product.id;

        return false;
    }

    return true;
}

/**
 * @brief Obtiene un producto de la base de datos mediante su ID.
 *
 * Ejecuta una consulta SQL para buscar un producto cuyo identificador
 * coincida con el valor recibido. Si el producto existe, sus datos se
 * almacenan en un objeto Product y posteriormente se devuelve.
 *
 * Si no se encuentra un producto con el ID proporcionado, se devuelve
 * un objeto Product con sus valores predeterminados.
 *
 * @param id Identificador único del producto que se desea obtener.
 *
 * @return Objeto Product con la información encontrada.
 * @return Un objeto Product vacío si no existe el producto o si ocurre
 * un error durante la consulta.
 */
Product ProductManager::getProduct(int id)
{
    Product product;

    QSqlQuery query;

    query.prepare(
        "SELECT id, name, barcode, purchase_price, sale_price "
        "FROM products "
        "WHERE id = ?"
        );

    query.addBindValue(id);

    if (query.exec())
    {
        if (query.next())
        {
            product.id = query.value("id").toInt();
            product.name = query.value("name").toString();
            product.barcode = query.value("barcode").toString();
            product.purchase_price = query.value("purchase_price").toDouble();
            product.sale_price = query.value("sale_price").toDouble();
        }
    }
    else
    {
        qDebug() << "Error getting product:"
                 << query.lastError().text();
    }

    return product;
}

/**
 * @brief Deletes a product from the database.
 *
 * Removes the product record from the products table using the product ID.
 * The function executes a DELETE SQL query and verifies if a record was
 * successfully removed.
 *
 * @param productId Unique identifier of the product to delete.
 *
 * @return true if the product was deleted successfully,
 *         false if the operation failed or no product was found.
 */
bool ProductManager::deleteProduct(int productId)
{
    QSqlQuery query;

    query.prepare(
        "DELETE FROM products "
        "WHERE id = ?"
        );

    query.addBindValue(productId);

    if (!query.exec())
    {
        qDebug() << "Delete product failed:"
                 << query.lastError().text();

        return false;
    }

    return query.numRowsAffected() > 0;
}
