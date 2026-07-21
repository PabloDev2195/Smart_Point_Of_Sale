#include "productRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


/**
 * @brief Constructs a ProductRepository object.
 */
ProductRepository::ProductRepository()
{
}


/**
 * @brief Adds a new product to the database.
 *
 * Inserts a new product record into the products table.
 *
 * @param product Product object containing the information to insert.
 *
 * @return true if the product was successfully added.
 * @return false if the database operation failed.
 */
bool ProductRepository::addProduct(const Product& product)
{
    QSqlQuery query;
    bool bAddProduct = true;

    query.prepare(
        "INSERT INTO products "
        "(name, barcode, purchase_price, sale_price) "
        "VALUES (?, ?, ?, ?)"
    );

    query.addBindValue(product.name);
    query.addBindValue(product.barcode);
    query.addBindValue(product.purchase_price);
    query.addBindValue(product.sale_price);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        bAddProduct = false;
    }

    return bAddProduct;
}


/**
 * @brief Retrieves all products from the database.
 *
 * Executes a query to obtain all stored products
 * from the products table and converts each record
 * into a Product object.
 *
 * @return QList<Product> List containing all products
 * retrieved from the database.
 */
QList<Product> ProductRepository::getProducts()
{
    QList<Product> products;

    QSqlQuery query(
        "SELECT id, name, barcode, purchase_price, sale_price "
        "FROM products"
    );


    while(query.next())
    {
        Product product;

        product.id = query.value("id").toInt();
        product.name = query.value("name").toString();
        product.barcode = query.value("barcode").toString();
        product.purchase_price = query.value("purchase_price").toDouble();
        product.sale_price = query.value("sale_price").toDouble();

        products.append(product);
    }


    return products;
}


/**
 * @brief Updates an existing product in the database.
 *
 * Updates the product information using the product ID
 * as the identifier.
 *
 * @param product Product object containing the updated information.
 *
 * @return true if the product was successfully updated.
 * @return false if the update operation failed or
 * no product was found with the given ID.
 */
bool ProductRepository::updateProduct(const Product& product)
{
    QSqlQuery query;
    bool bUpdateProduct = true;

    query.prepare(
        "UPDATE products "
        "SET barcode = ?, "
        "name = ?, "
        "purchase_price = ?, "
        "sale_price = ? "
        "WHERE id = ?"
    );

    query.addBindValue(product.barcode);
    query.addBindValue(product.name);
    query.addBindValue(product.purchase_price);
    query.addBindValue(product.sale_price);
    query.addBindValue(product.id);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        bUpdateProduct = false;
    }

    if(query.numRowsAffected() == 0)
    {
        bUpdateProduct = false;
    }

    return bUpdateProduct;
}


/**
 * @brief Deletes a product from the database.
 *
 * Removes a product record from the products table
 * using the product ID.
 *
 * @param id Identifier of the product to delete.
 *
 * @return true if the product was successfully deleted.
 * @return false if the delete operation failed or
 * no product was found with the given ID.
 */
bool ProductRepository::deleteProduct(int id)
{
    QSqlQuery query;
    bool bDeleteProduct = true;

    query.prepare(
        "DELETE FROM products "
        "WHERE id = ?"
    );

    query.addBindValue(id);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        bDeleteProduct = false;
    }

    if(query.numRowsAffected() == 0)
    {
        bDeleteProduct = false;
    }

    return bDeleteProduct;
}