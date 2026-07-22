#ifndef DIALOG_ADDPRODUCT_H
#define DIALOG_ADDPRODUCT_H

#include <QDialog>

namespace Ui {
class Dialog_AddProduct;
}

class Dialog_AddProduct : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_AddProduct(QWidget *parent = nullptr);
    ~Dialog_AddProduct();

private:
    Ui::Dialog_AddProduct *ui;
};

#endif // DIALOG_ADDPRODUCT_H
