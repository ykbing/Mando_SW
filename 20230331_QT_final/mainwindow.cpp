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

    // Input Mode LineEdit Init 0
    ui->input_value1->setText("0");
    ui->input_value2->setText("0");
    ui->input_value3->setText("0");
    ui->input_value4->setText("0");
    ui->input_value5->setText("0");
    ui->input_value6->setText("0");
    ui->input_value7->setText("0");
    ui->input_value8->setText("0");

//    // Input Mode Send
//    QObject::connect(ui->sendBtn_2, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));

    // Input Mode 0, 1
    QObject::connect(ui->btn1, SIGNAL(clicked()), this, SLOT(btn1_clicked()));
    connect(ui->btn1, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn2, SIGNAL(clicked()), this, SLOT(btn2_clicked()));
    connect(ui->btn2, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn3, SIGNAL(clicked()), this, SLOT(btn3_clicked()));
    connect(ui->btn3, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn4, SIGNAL(clicked()), this, SLOT(btn4_clicked()));
    connect(ui->btn4, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn5, SIGNAL(clicked()), this, SLOT(btn5_clicked()));
    connect(ui->btn5, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn6, SIGNAL(clicked()), this, SLOT(btn6_clicked()));
    connect(ui->btn6, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn7, SIGNAL(clicked()), this, SLOT(btn7_clicked()));
    connect(ui->btn7, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));
    connect(ui->btn8, SIGNAL(clicked()), this, SLOT(btn8_clicked()));
    connect(ui->btn8, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));

    // Input Mode Clear
    QObject::connect(ui->clearBtn_2, SIGNAL(clicked()), this, SLOT(clearBtn2_clicked()));
    connect(ui->clearBtn_2, SIGNAL(clicked()), this, SLOT(sendBtn2_clicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

//******* Clicked Serial Open Button *******//
void MainWindow::on_openBtn_clicked(void)
{
    setPort->setPortName("ttyUSB1"); // 포트 번호 설정
    setPort->setBaudRate(QSerialPort::Baud115200); // 전송 속도 설정
//    setPort->setDataBits(QSerialPort::Data8); // 데이터 비트 설정
//    setPort->setParity(QSerialPort::NoParity); // 패리티 비트 설정
//    setPort->setStopBits(QSerialPort::OneStop); // 스탑 비트 설정
//    setPort->setFlowControl(QSerialPort::NoFlowControl); // 흐름 제어 설정

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

const int numElements = 9;  // 8개의 값을 담을 수 있는 배열 크기
unsigned char receivedValues[numElements] = {0,};
unsigned char OUTPUT_buff[numElements] = {0,};

//******* Init_array *******//
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

//******* Receive Data *******//
void MainWindow::text_reading(void)
{
    init_array();
    QByteArray data2 = setPort->readAll();
    int rx_data = 0, adc_data;

    const DATA2* received = reinterpret_cast<const DATA2*>(data2.data());
    int receivedValues[numElements];

    for (int i = 0; i < numElements; i++)
    {
        receivedValues[i] = received->output_dataArr[i];
        //qDebug() << i << receivedValues[i];


        if(receivedValues[0]==174 && receivedValues[1]==184)
        {
            if(i == 4)
            {
              rx_data = receivedValues[i];
            }

            ADC_val.a[0] = receivedValues[5];
            ADC_val.a[1] = receivedValues[6];
            adc_data = ADC_val.data;

            //qDebug() << i << receivedValues[i];
            //qDebug() << rx_data << adc_data;

        }
    }


//    if (data2.size() == sizeof(DATA2)) // 데이터 크기 확인
//    {
//        const DATA2* received = reinterpret_cast<const DATA2*>(data2.data());
//        int receivedValues[numElements];


//        for (int i = 0; i < numElements; i++)
//        {
//            receivedValues[i] = received->output_dataArr[i];
//            qDebug() << receivedValues[i];

//            // PushButton Data
//            if(i == 4)
//            {
//              rx_data = receivedValues[i];
//            }

//            // ADC Data
//            ADC_val.a[0] = receivedValues[5];
//            ADC_val.a[1] = receivedValues[6];
//            adc_data = ADC_val.data;
//        }
//    }
//    else // 데이터 크기가 잘못되었거나 이상한 값이 들어온 경우
//    {
//        qDebug() << "error";
//        rx_data = 0;
//    }

//    qDebug() << rx_data << adc_data;

    QString ri_value = QString::number(rx_data);
    ui->r_value->setText(ri_value);

    for(int i = 7 ; i >= 0 ; --i)
    {
        OUTPUT_buff[i] = (rx_data>>i)&1;
    }

    QFrame* frames[8] = {ui->led8, ui->led7, ui->led6, ui->led5,
    ui->led4, ui->led3, ui->led2, ui->led1};

    for(int i= 0; i < 8 ; ++i)
    {
        if(OUTPUT_buff[i]==0)    frames[i]->setStyleSheet("background-color: red;");
        else                     frames[i]->setStyleSheet("background-color: green;");
    }

    QString adcValue = QString::number(adc_data);
    ui->adc_value->setText(adcValue);
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
    // 변수를 선언하고 초기값을 설정
    QString inputData, inputData2;
    int oac=0, i=0, n, strTonum;

    // 입력된 8개의 값을 inputData 문자열에 추가
    inputData+=ui->input_value8->text();
    inputData+=ui->input_value7->text();
    inputData+=ui->input_value6->text();
    inputData+=ui->input_value5->text();
    inputData+=ui->input_value4->text();
    inputData+=ui->input_value3->text();
    inputData+=ui->input_value2->text();
    inputData+=ui->input_value1->text();

    // 입력된 값을 텍스트 형태로 출력
    ui->result_view->setText(inputData);
    // 문자열을 정수형으로 변환
    strTonum = inputData.toInt();

    // 2진수를 10진수로 변환
    while (strTonum != 0)
    {
        n = strTonum % 10;
        strTonum /= 10;
        oac += n * pow(2, i);
        ++i;
    }
    // 10진수를 문자열로 변환
    inputData2 = inputData2.setNum(oac);

    // 변환된 값을 출력
    ui->result_view_2->setText(inputData2);

    // 데이터 구조체를 생성
    DATA *send = new DATA;
    // 입력된 값을 구조체 멤버 변수에 저장
    send->input_dataArr = (unsigned char)inputData2.toDouble();
    // 패킷을 생성
    auto packet = reinterpret_cast<char*>(send);
    // 패킷을 시리얼 포트에 사용
    setPort->write(packet,sizeof (DATA));
    // 디버그 출력
    qDebug()<<"Go to MEGA 2560";
    qDebug()<< inputData2;
    // 동적으로 생성한 구조체를 삭제
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
