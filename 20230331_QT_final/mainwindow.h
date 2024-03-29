#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

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
    void clearBtn2_clicked(void);
    void sendBtn2_clicked(void);
    void on_openBtn_clicked(void);
    void on_stopBtn_clicked(void);
    void text_reading(void);
    void init_array(void);


private:
    Ui::MainWindow *ui;
    QSerialPort *setPort;
    struct DATA{
        unsigned char input_dataArr;
    };
    struct DATA2{
        unsigned char output_dataArr[9];
    };
    union
    {
        uint16_t data;
        uint8_t a[2];
    }ADC_val;
};

#endif // MAINWINDOW_H
