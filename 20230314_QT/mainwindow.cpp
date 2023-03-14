#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->aBtn->setChecked(true);
    ui->cBtn->setChecked(true);
    ui->textView->setEnabled(false);

    connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(slotBtnOK()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(slotBtnClear()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotBtnOk(void)
{
    QString strResult;

    if(ui->aBtn->isChecked())
        strResult = ui->aBtn->text();
    else
        strResult = ui->bBtn->text();

    strResult += "+";

    if(ui->cBtn->isChecked())
        strResult += ui->cBtn->text();
    else
        strResult += ui->dBtn->text();

    ui->textView->setText(strResult);
}

void MainWindow::slotBtnClear(void)
{
    ui->textView->clear();
}
