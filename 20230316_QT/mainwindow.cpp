#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Output Mode Send
    QObject::connect(ui->sendBtn, SIGNAL(clicked()), this, SLOT(sendBtn_clicked()));

    // Output Mode Clear
    QObject::connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(clearBtn_clicked()));

    // Send Mode LED check
    QObject::connect(ui->ledBtn, SIGNAL(clicked()), this, SLOT(updateFrames()));

    // Input Mode 0, 1
    QObject::connect(ui->btn1, SIGNAL(clicked()), this, SLOT(btn1_clicked()));
    QObject::connect(ui->btn2, SIGNAL(clicked()), this, SLOT(btn2_clicked()));
    QObject::connect(ui->btn3, SIGNAL(clicked()), this, SLOT(btn3_clicked()));
    QObject::connect(ui->btn4, SIGNAL(clicked()), this, SLOT(btn4_clicked()));
    QObject::connect(ui->btn5, SIGNAL(clicked()), this, SLOT(btn5_clicked()));
    QObject::connect(ui->btn6, SIGNAL(clicked()), this, SLOT(btn6_clicked()));
    QObject::connect(ui->btn7, SIGNAL(clicked()), this, SLOT(btn7_clicked()));
    QObject::connect(ui->btn8, SIGNAL(clicked()), this, SLOT(btn8_clicked()));

    // Input Mode Clear
    QObject::connect(ui->clearBtn_2, SIGNAL(clicked()), this, SLOT(clearBtn2_clicked()));

    // Input Mode Send
    QObject::connect(ui->sendBtn_2, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//******* Change LED Color *******//
void MainWindow::updateFrames(void)
{


    // HEX_INPUT의 입력값을 가져옴
    QString hexValue = ui->hex_value->text();

    // 입력값을 10자리의 2진수 문자열로 변환
    QString binaryValue = QString("%1").arg(hexValue.toUInt(nullptr, 10), 8, 2, QLatin1Char('0'));

    // 변환된 2진수 문자열에서 각 자리의 값을 배열에 저장
    int values[8];
    for (int i = 0; i < 8; i++) {
    values[i] = binaryValue.mid(i, 1).toInt();
    }


    // frame 포인터 배열
    QFrame* frames[8] = {ui->led1, ui->led2, ui->led3, ui->led4,
    ui->led5, ui->led6, ui->led7, ui->led8};

    // 배열의 값을 기반으로 frame 색상 설정
    for (int i = 0; i < 8; i++)
    {
        if (values[i] == 0)
        {
            frames[i]->setStyleSheet("background-color: red;");
        }
        else
        {
            frames[i]->setStyleSheet("background-color: blue;");
        }
    }
}

//******* Output Send Value *******//
void MainWindow::sendBtn_clicked(void)
{
    QString myStr2;
    myStr2=ui->hex_value->text();
    ui->hex_value->setText(myStr2);
}

//******* Output Value Clear *******//
void MainWindow::clearBtn_clicked(void)
{
    ui->hex_value->clear();
}

//******* Input Value Clear *******//
void MainWindow::clearBtn2_clicked(void)
{
    clearValue_1();
    clearValue_2();
    clearValue_3();
    clearValue_4();
    clearValue_5();
    clearValue_6();
    clearValue_7();
    clearValue_8();
}

//******* Input Send Value  *******//
void MainWindow::sendBtn2_clicked(void)
{
    QString myStr;

    myStr+="Input Data: ";
    myStr+=ui->input_value1->text();
    myStr+=ui->input_value2->text();
    myStr+=ui->input_value3->text();
    myStr+=ui->input_value4->text();
    myStr+=ui->input_value5->text();
    myStr+=ui->input_value6->text();
    myStr+=ui->input_value7->text();
    myStr+=ui->input_value8->text();

    ui->result_view->setText(myStr);
}


//******* Input Mode Value *******//
void MainWindow::btn1_clicked(void)
{
    static bool isOn = false;

    if(isOn) {
        clearValue_1();
    }
    else {
        inputValue_1();
    }
    isOn = !isOn;
}

void MainWindow::clearValue_1(void)
{
    ui->input_value1->setText("0");
}

void MainWindow::inputValue_1(void)
{
    ui->input_value1->setText("1");
}

void MainWindow::btn2_clicked(void)
{
    static bool isOn_2 = false;

    if(isOn_2) {
        clearValue_2();
    }
    else {
        inputValue_2();
    }
    isOn_2 = !isOn_2;
}

void MainWindow::clearValue_2(void)
{
    ui->input_value2->setText("0");
}

void MainWindow::inputValue_2(void)
{
    ui->input_value2->setText("1");
}
void MainWindow::btn3_clicked(void)
{
    static bool isOn_3 = false;

    if(isOn_3) {
        clearValue_3();
    }
    else {
        inputValue_3();
    }
    isOn_3 = !isOn_3;
}

void MainWindow::clearValue_3(void)
{
    ui->input_value3->setText("0");
}

void MainWindow::inputValue_3(void)
{
    ui->input_value3->setText("1");
}

void MainWindow::btn4_clicked(void)
{
    static bool isOn_4 = false;

    if(isOn_4) {
        clearValue_4();
    }
    else {
        inputValue_4();
    }
    isOn_4 = !isOn_4;
}

void MainWindow::clearValue_4(void)
{
    ui->input_value4->setText("0");
}

void MainWindow::inputValue_4(void)
{
    ui->input_value4->setText("1");
}
void MainWindow::btn5_clicked(void)
{
    static bool isOn_5 = false;

    if(isOn_5) {
        clearValue_5();
    }
    else {
        inputValue_5();
    }
    isOn_5 = !isOn_5;
}

void MainWindow::clearValue_5(void)
{
    ui->input_value5->setText("0");
}

void MainWindow::inputValue_5(void)
{
    ui->input_value5->setText("1");
}
void MainWindow::btn6_clicked(void)
{
    static bool isOn_6 = false;

    if(isOn_6) {
        clearValue_6();
    }
    else {
        inputValue_6();
    }
    isOn_6 = !isOn_6;
}

void MainWindow::clearValue_6(void)
{
    ui->input_value6->setText("0");
}

void MainWindow::inputValue_6(void)
{
    ui->input_value6->setText("1");
}
void MainWindow::btn7_clicked(void)
{
    static bool isOn_7 = false;

    if(isOn_7) {
        clearValue_7();
    }
    else {
        inputValue_7();
    }
    isOn_7 = !isOn_7;
}

void MainWindow::clearValue_7(void)
{
    ui->input_value7->setText("0");
}

void MainWindow::inputValue_7(void)
{
    ui->input_value7->setText("1");
}

void MainWindow::btn8_clicked(void)
{
    static bool isOn_8 = false;

    if(isOn_8) {
        clearValue_8();
    }
    else {
        inputValue_8();
    }
    isOn_8 = !isOn_8;
}

void MainWindow::clearValue_8(void)
{
    ui->input_value8->setText("0");
}

void MainWindow::inputValue_8(void)
{
    ui->input_value8->setText("1");
}
//******* Input Mode Value *******//
