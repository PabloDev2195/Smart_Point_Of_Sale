#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

/**
 * @class Product
 *
 * @brief Represents a product entity.
 *
 * This class defines the data model used to represent
 * product information within the application.
 *
 * Product objects are used by the repository layer to
 * store and retrieve product data from the database.
 */
class Product
{
public:

    /**
     * @brief Unique identifier of the product.
     */
    int id = 0;


    /**
     * @brief Product name.
     */
    QString name;


    /**
     * @brief Unique barcode identifier of the product.
     */
    QString barcode;


    /**
     * @brief Product purchase price.
     */
    double purchase_price = 0.0;


    /**
     * @brief Product sale price.
     */
    double sale_price = 0.0;
};

#endif // PRODUCT_H