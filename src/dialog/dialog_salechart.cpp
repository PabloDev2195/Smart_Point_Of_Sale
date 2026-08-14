#include "dialog_salechart.h"
#include "ui_dialog_salechart.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QPainter>

/**
 * @brief Constructs the sales chart dialog.
 *
 * Initializes the dialog and creates a bar chart displaying daily
 * sales totals and gross profits.
 *
 * The chart data is provided through the DailySales list. Each entry
 * represents the sales information for a specific day.
 *
 * The dialog also displays the initial and final dates corresponding
 * to the provided data range.
 *
 * @param data List of daily sales statistics used to generate the chart.
 * @param parent Parent widget of the dialog.
 */
Dialog_SaleChart::Dialog_SaleChart(
    const QList<DailySales>& data,
    QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog_SaleChart)
    , m_data(data)
{
    ui->setupUi(this);

    QBarSet *salesSet = new QBarSet("Total");
    QBarSet *profitSet = new QBarSet("Ganancias");

    QStringList categories;

    double maxValue = 0.0;

    /*
     * Populate the chart datasets with the daily sales
     * and gross profit information.
     */
    for (const DailySales& day : m_data)
    {
        *salesSet << day.total;
        *profitSet << day.grossProfit;

        categories << day.date.toString("dd/MM");

        maxValue = qMax(maxValue, day.total);
        maxValue = qMax(maxValue, day.grossProfit);
    }

    /*
     * Display the selected date range in the dialog header.
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
     * Create and configure the bar series.
     */
    QBarSeries *series = new QBarSeries();

    series->append(salesSet);
    series->append(profitSet);

    series->setLabelsVisible(true);
    series->setLabelsFormat("$@value");

    /*
     * Configure the chart.
     */
    QChart *chart = new QChart();

    chart->addSeries(series);

    chart->setTitle("Ventas y ganancias por día");

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    /*
     * Configure the X axis with the dates represented
     * by each group of bars.
     */
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    axisX->append(categories);
    axisX->setTitleText("Fecha");

    /*
     * Configure the Y axis according to the maximum value
     * found in the sales and profit data.
     */
    QValueAxis *axisY = new QValueAxis();

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

    /*
     * Add the axes to the chart and associate them
     * with the bar series.
     */
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    /*
     * Display the configured chart in the graphics view.
     */
    ui->graphicsView_Chart->setChart(chart);

    ui->graphicsView_Chart->setRenderHint(
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