#ifndef SPOS_MAINWINDOW_H
#define SPOS_MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class SPOS_MainWindow;
}
QT_END_NAMESPACE

class SPOS_MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit SPOS_MainWindow(QWidget *parent = nullptr);
        ~SPOS_MainWindow() override;

    private:
        Ui::SPOS_MainWindow *ui;

        QStandardItemModel *m_model;

    private slots:
        void on_action_AddNewProduct_triggered();

        void on_action_ModifyProduct_triggered();

        void on_action_DeleteProduct_triggered();

        void on_pushButton_FindProduct_clicked();
};
#endif // SPOS_MAINWINDOW_H
