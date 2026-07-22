#ifndef DIALOG_MODIFYPRODUCT_H
#define DIALOG_MODIFYPRODUCT_H

#include <QDialog>

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
};

#endif // DIALOG_MODIFYPRODUCT_H
