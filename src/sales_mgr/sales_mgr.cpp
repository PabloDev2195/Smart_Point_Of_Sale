#include "sales_mgr.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * @brief Retrieves all completed sales from the database.
 *
 * Executes a query against the sales table and converts each
 * database record into a SaleRecord object.
 *
 * @return QList<SaleRecord> List of completed sales.
 */
QList<SaleRecord> SalesMgr::getSales()
{
    QList<SaleRecord> sales;

    QSqlQuery query;

    query.prepare(
        "SELECT id, "
        "ticket_number, "
        "sale_date, "
        "total, "
        "gross_profit, "
        "net_profit "
        "FROM sales "
        "ORDER BY ticket_number DESC"
        );

    if(!query.exec())
    {
        qDebug() << "Get sales error:"
                 << query.lastError().text();

        return sales;
    }

    while(query.next())
    {
        SaleRecord sale;

        sale.id = query.value("id").toInt();
        sale.ticketNumber = query.value("ticket_number").toInt();
        sale.saleDate = query.value("sale_date").toString();
        sale.total = query.value("total").toDouble();
        sale.grossProfit = query.value("gross_profit").toDouble();
        sale.netProfit = query.value("net_profit").toDouble();

        sales.append(sale);
    }

    return sales;
}

/**
 * @brief Retrieves a completed sale by ticket number.
 *
 * Executes a parameterized query against the sales table
 * using the ticket number as the search criterion.
 *
 * @param ticketNumber Ticket number of the sale.
 *
 * @return SaleRecord Sale information.
 * @return Empty SaleRecord if the sale is not found.
 */
SaleRecord SalesMgr::getSale(int ticketNumber)
{
    SaleRecord sale;

    QSqlQuery query;

    query.prepare(
        "SELECT id, "
        "ticket_number, "
        "sale_date, "
        "total, "
        "gross_profit, "
        "net_profit "
        "FROM sales "
        "WHERE ticket_number = ?"
        );

    query.addBindValue(ticketNumber);

    if(!query.exec())
    {
        qDebug() << "Get sale error:"
                 << query.lastError().text();

        return sale;
    }

    if(query.next())
    {
        sale.id = query.value("id").toInt();
        sale.ticketNumber = query.value("ticket_number").toInt();
        sale.saleDate = query.value("sale_date").toString();
        sale.total = query.value("total").toDouble();
        sale.grossProfit = query.value("gross_profit").toDouble();
        sale.netProfit = query.value("net_profit").toDouble();
    }

    return sale;
}

/**
 * @brief Retrieves all items belonging to a completed sale.
 *
 * Joins the sale_items and products tables to retrieve
 * the product information together with the values recorded
 * for the completed sale.
 *
 * @param saleId Database ID of the completed sale.
 *
 * @return QList<SaleItemRecord> List of sale items.
 */
QList<SaleItemRecord> SalesMgr::getSaleItems(int saleId)
{
    QList<SaleItemRecord> items;

    QSqlQuery query;

    query.prepare(
        "SELECT si.product_id, "
        "p.name, "
        "p.barcode, "
        "si.quantity, "
        "si.unit_price, "
        "si.subtotal "
        "FROM sale_items si "
        "INNER JOIN products p "
        "ON si.product_id = p.id "
        "WHERE si.sale_id = ?"
        );

    query.addBindValue(saleId);

    if(!query.exec())
    {
        qDebug() << "Get sale items error:"
                 << query.lastError().text();

        return items;
    }

    while(query.next())
    {
        SaleItemRecord item;

        item.productId = query.value("product_id").toInt();
        item.productName = query.value("name").toString();
        item.barcode = query.value("barcode").toString();
        item.quantity = query.value("quantity").toDouble();
        item.unitPrice = query.value("unit_price").toDouble();
        item.subtotal = query.value("subtotal").toDouble();

        items.append(item);
    }

    return items;
}