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

public slots:
    void btn1_clicked(void);
    void clearValue_1(void);
    void inputValue_1(void);
    void btn2_clicked(void);
    void clearValue_2(void);
    void inputValue_2(void);
    void btn3_clicked(void);
    void clearValue_3(void);
    void inputValue_3(void);
    void btn4_clicked(void);
    void clearValue_4(void);
    void inputValue_4(void);
    void btn5_clicked(void);
    void clearValue_5(void);
    void inputValue_5(void);
    void btn6_clicked(void);
    void clearValue_6(void);
    void inputValue_6(void);
    void btn7_clicked(void);
    void clearValue_7(void);
    void inputValue_7(void);
    void btn8_clicked(void);
    void clearValue_8(void);
    void inputValue_8(void);
    void clearBtn_clicked(void);
    void sendBtn_clicked(void);
    void clearBtn2_clicked(void);
    void sendBtn2_clicked(void);
    void updateFrames(void);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
