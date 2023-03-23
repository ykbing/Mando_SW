#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QByteArray>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // SerialPort Setting
    setPort = new QSerialPort(this);

    // Output Mode Receive Data
    QObject::connect(setPort, SIGNAL(readyRead()), this, SLOT(text_reading()));

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

//******* Clicked Serial Open Button *******//
void MainWindow::on_openBtn_clicked(void)
{
    setPort->setPortName("ttyUSB0"); // 포트 번호 설정
    setPort->setBaudRate(QSerialPort::Baud115200); // 전송 속도 설정
    setPort->setDataBits(QSerialPort::Data8); // 데이터 비트 설정
    setPort->setParity(QSerialPort::NoParity); // 패리티 비트 설정
    setPort->setStopBits(QSerialPort::OneStop); // 스탑 비트 설정
    setPort->setFlowControl(QSerialPort::NoFlowControl); // 흐름 제어 설정

    // 시리얼 포트 열기
    if (!setPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open serial port";
        return;
    }
    else{
        qDebug() << "Success to open serial port";
    }
}

//******* Clicked Serial Stop Button *******//
void MainWindow::on_stopBtn_clicked(void)
{
    setPort->close();
    qDebug() << "Closed serial port";
}

const int numElements = 16;  // 8개의 값을 담을 수 있는 배열 크기
unsigned int receivedValues[numElements] = {0,};
unsigned int OUTPUT_buff[numElements] = {0,};

//******* Binary calculate *******//
void MainWindow::init_array(void)
{
    for(int i=0; i<16; i++)
    {
        OUTPUT_buff[i] = 0;
    }
}

//******* Binary calculate *******//
void calculate(int data, int count)
{
    for(int i = 0; i < count;i++)
    {
        OUTPUT_buff[i] = data % 2;
        data /= 2;
    }
}

int MainWindow::map_c(int data, int in_min,int in_max,int out_min,int out_max )
{
    return (data - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//******* Receive Data *******//
void MainWindow::text_reading(void)
{
    init_array();
    QByteArray data2 = setPort->readAll();
    int rx_data = 0, adc_data;

    if (data2.size() == sizeof(DATA2)) // 데이터 크기 확인
    {
        const DATA2* received = reinterpret_cast<const DATA2*>(data2.data());
        int receivedValues[numElements];

        for (int i = 0; i < numElements; i++)
        {
            receivedValues[i] = received->output_dataArr[i];
            qDebug() << receivedValues[i];

            if(i == 4)
            {
              rx_data = receivedValues[i];
            }

            if(i == 12)
            {
              adc_data = receivedValues[i];
            }
        }
    }
    else // 데이터 크기가 잘못되었거나 이상한 값이 들어온 경우
    {
        //qDebug() << "error";
    }

    QString ri_value = QString::number(rx_data);
    ui->r_value->setText(ri_value);

    adc_data = map_c(adc_data,0,255,0,4096);
    QString adcValue = QString::number(adc_data);
    ui->adc_value->setText(adcValue);

    if(rx_data >= 128)                     calculate(rx_data,8);
    else if(rx_data >= 64 && rx_data < 128)   calculate(rx_data,7);
    else if(rx_data >= 32 && rx_data < 64)    calculate(rx_data,6);
    else if(rx_data >= 16 && rx_data < 32)    calculate(rx_data,5);
    else if(rx_data >= 8  && rx_data < 16)    calculate(rx_data,4);
    else if(rx_data >= 4  && rx_data < 8 )    calculate(rx_data,3);
    else if(rx_data >= 2  && rx_data < 4 )    calculate(rx_data,2);
    else if(rx_data == 1)                  OUTPUT_buff[0] = 1;

    QFrame* frames[8] = {ui->led8, ui->led7, ui->led6, ui->led5,
    ui->led4, ui->led3, ui->led2, ui->led1};

    for (int i = 0; i < 8; i++)
    {
        qDebug() <<  OUTPUT_buff[i];
    }

    (OUTPUT_buff[0] == 1) ?  frames[7]->setStyleSheet("background-color: green;") : frames[7]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[1] == 1) ?  frames[6]->setStyleSheet("background-color: green;") : frames[6]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[2] == 1) ?  frames[5]->setStyleSheet("background-color: green;") : frames[5]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[3] == 1) ?  frames[4]->setStyleSheet("background-color: green;") : frames[4]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[4] == 1) ?  frames[3]->setStyleSheet("background-color: green;") : frames[3]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[5] == 1) ?  frames[2]->setStyleSheet("background-color: green;") : frames[2]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[6] == 1) ?  frames[1]->setStyleSheet("background-color: green;") : frames[1]->setStyleSheet("background-color: red;");
    (OUTPUT_buff[7] == 1) ?  frames[0]->setStyleSheet("background-color: green;") : frames[0]->setStyleSheet("background-color: red;");
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
    QString inputData, inputData2;
    int oac=0, i=0, n, strTonum;

    inputData+=ui->input_value8->text();
    inputData+=ui->input_value7->text();
    inputData+=ui->input_value6->text();
    inputData+=ui->input_value5->text();
    inputData+=ui->input_value4->text();
    inputData+=ui->input_value3->text();
    inputData+=ui->input_value2->text();
    inputData+=ui->input_value1->text();

    ui->result_view->setText(inputData);

    strTonum = inputData.toInt();

        while (strTonum != 0) {
            n = strTonum % 10;
            strTonum /= 10;
            oac += n * pow(2, i);
            ++i;
        }

    inputData2 = inputData2.setNum(oac);

    ui->result_view_2->setText(inputData2);

    DATA *send = new DATA;

    send->input_dataArr = (unsigned char)inputData2.toDouble();

    auto packet = reinterpret_cast<char*>(send);
    setPort->write(packet,sizeof (DATA));
    qDebug()<<"\n Go to Aurix TC275";

    delete send;
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
