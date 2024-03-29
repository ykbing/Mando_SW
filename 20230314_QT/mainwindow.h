#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void slotBtnOk(void);
    void slotBtnClear(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
