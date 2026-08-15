#include "dialog_salechart.h"
#include "ui_dialog_salechart.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QHorizontalBarSeries>

#include <QPainter>

/**
 * @brief Constructs the sales chart dialog.
 *
 * Initializes the dialog and creates bar charts displaying daily
 * sales totals, gross profits, and products sold.
 *
 * The charts use the provided sales statistics and product sales
 * data corresponding to the selected date range.
 *
 * The dialog also displays the initial and final dates corresponding
 * to the provided data range.
 *
 * @param data List of daily sales statistics used to generate the
 *             sales and profit chart.
 * @param productSales List of product sales statistics used to
 *                     generate the products chart.
 * @param parent Parent widget of the dialog.
 */
Dialog_SaleChart::Dialog_SaleChart(
    const QList<DailySales>& data,
    const QList<ProductSales>& productSales,
    QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_SaleChart)
    , m_data(data)
    , m_productSales(productSales)
{
    ui->setupUi(this);

    /*
     * Display the selected date range.
     */
    if (!m_data.isEmpty())
    {
        ui->label_DateInit->setText(
            m_data.first().date.toString("dd/MM/yyyy"));

        ui->label_DateFinal->setText(
            m_data.last().date.toString("dd/MM/yyyy"));
    }
    else
    {
        ui->label_DateInit->setText("-");
        ui->label_DateFinal->setText("-");
    }

    /*
     * ============================================================
     * Daily sales and profit chart
     * ============================================================
     */

    QBarSet *salesSet =
        new QBarSet("Total");

    QBarSet *profitSet =
        new QBarSet("Ganancias");

    QStringList categories;

    double maxValue = 0.0;

    for (const DailySales& day : m_data)
    {
        *salesSet << day.total;
        *profitSet << day.grossProfit;

        categories << day.date.toString("dd/MM");

        maxValue = qMax(maxValue, day.total);
        maxValue = qMax(maxValue, day.grossProfit);
    }

    QBarSeries *series =
        new QBarSeries();

    series->append(salesSet);
    series->append(profitSet);

    series->setLabelsVisible(true);
    series->setLabelsFormat("$@value");

    QChart *chart =
        new QChart();

    chart->addSeries(series);

    chart->setTitle(
        "Ventas y ganancias por día");

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(
        Qt::AlignBottom);

    QBarCategoryAxis *axisX =
        new QBarCategoryAxis();

    axisX->append(categories);
    axisX->setTitleText("Fecha");

    QValueAxis *axisY =
        new QValueAxis();

    axisY->setTitleText("Cantidad");
    axisY->setLabelFormat("$ %.0f");
    axisY->setMin(0);

    if (maxValue > 0)
    {
        axisY->setMax(maxValue * 1.2);
    }
    else
    {
        axisY->setMax(100);
    }

    chart->addAxis(
        axisX,
        Qt::AlignBottom);

    chart->addAxis(
        axisY,
        Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    ui->graphicsView_Chart->setChart(chart);

    ui->graphicsView_Chart->setRenderHint(
        QPainter::Antialiasing);


    /*
     * ============================================================
     * Products sold chart
     * ============================================================
     */

    QBarSet *productSet =
        new QBarSet("Cantidad vendida");

    QStringList productCategories;

    double maxProductQuantity = 0.0;

    for (const ProductSales& product : m_productSales)
    {
        *productSet << product.quantity;

        productCategories << product.productName;

        maxProductQuantity =
            qMax(maxProductQuantity,
                 product.quantity);
    }

    QHorizontalBarSeries *productSeries =
        new QHorizontalBarSeries();

    productSeries->append(productSet);

    productSeries->setLabelsVisible(true);
    productSeries->setLabelsFormat("@value");

    QChart *productChart =
        new QChart();

    productChart->addSeries(productSeries);

    productChart->setTitle(
        "Productos vendidos");

    productChart->legend()->setVisible(false);

    QBarCategoryAxis *productAxisY =
        new QBarCategoryAxis();

    productAxisY->append(productCategories);
    productAxisY->setTitleText("Producto");

    QValueAxis *productAxisX =
        new QValueAxis();

    productAxisX->setTitleText("Cantidad");
    productAxisX->setLabelFormat("%.0f");
    productAxisX->setMin(0);

    if (maxProductQuantity > 0)
    {
        productAxisX->setMax(
            maxProductQuantity * 1.2);
    }
    else
    {
        productAxisX->setMax(10);
    }

    productChart->addAxis(
        productAxisX,
        Qt::AlignBottom);

    productChart->addAxis(
        productAxisY,
        Qt::AlignLeft);

    productSeries->attachAxis(
        productAxisX);

    productSeries->attachAxis(
        productAxisY);

    ui->graphicsView_Products->setChart(
        productChart);

    ui->graphicsView_Products->setRenderHint(
        QPainter::Antialiasing);
}

/**
 * @brief Destroys the sales chart dialog.
 *
 * Releases the resources allocated by the UI.
 */
Dialog_SaleChart::~Dialog_SaleChart()
{
    delete ui;
}