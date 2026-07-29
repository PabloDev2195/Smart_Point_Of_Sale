#ifndef DIALOG_DELETEPRODUCT_H
#define DIALOG_DELETEPRODUCT_H

#include <QDialog>

namespace Ui {
class Dialog_DeleteProduct;
}

class Dialog_DeleteProduct : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_DeleteProduct(QWidget *parent = nullptr);
    ~Dialog_DeleteProduct();

private slots:
    void on_pushButton_Find_clicked();

    void on_tableWidget_Products_cellClicked(int row, int column);

    void on_pushButton_Delete_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::Dialog_DeleteProduct *ui;

    int m_selectedProductId = -1;

};

#endif // DIALOG_DELETEPRODUCT_H
