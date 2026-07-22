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

private:
    Ui::Dialog_DeleteProduct *ui;
};

#endif // DIALOG_DELETEPRODUCT_H
