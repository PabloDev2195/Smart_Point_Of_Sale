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
};
#endif // SPOS_MAINWINDOW_H
