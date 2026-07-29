#ifndef DIALOG_MODIFYPRODUCT_H
#define DIALOG_MODIFYPRODUCT_H

#include <QDialog>
#include "../models/Product.h"

namespace Ui {
class Dialog_ModifyProduct;
}

class Dialog_ModifyProduct : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ModifyProduct(QWidget *parent = nullptr);
    ~Dialog_ModifyProduct();

private:
    Ui::Dialog_ModifyProduct *ui;

    Product m_product;

private slots:
    void loadSelectedProduct(int row, int column);

    void on_pushButton_Find_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_Cancel_clicked();
};

#endif // DIALOG_MODIFYPRODUCT_H
