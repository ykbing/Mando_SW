#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->input_text->setText("InputValue");
    ui->clearBtn->setText("Clear");

    QObject::connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(slotClearText()));
    connect(ui->inputBtn, SIGNAL(clicked()), this, SLOT(slotSetText()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotClearText(void)  // Clean View
{
    ui->textView->clear();
}

void MainWindow::slotSetText(void)   // Input Text View
{
    QString myStr;
    myStr = ui->input_value->text();
    ui->textView->setText(myStr);
}
