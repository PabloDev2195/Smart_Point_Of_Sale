#ifndef SPOS_MAINWINDOW_H
#define SPOS_MAINWINDOW_H

#include <QMainWindow>

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

    private slots:
        void on_action_AddNewProduct_triggered();

        void on_action_ModifyProduct_triggered();

        void on_action_DeleteProduct_triggered();
};
#endif // SPOS_MAINWINDOW_H
