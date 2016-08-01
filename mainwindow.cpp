#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonHello,SIGNAL(clicked(bool)),&_client,SLOT(sendHello()));
    connect(&_client, SIGNAL(log(QString)), this, SLOT(log(QString)));
    connect(ui->lineEditHost,SIGNAL(textChanged(QString)),this,SLOT(hostNameEdited()));
    connect(ui->lineEditPort,SIGNAL(textChanged(QString)),this,SLOT(portEdited()));
    ui->pushButtonHello->setEnabled(true);
    ui->lineEditHost->setText("localhost");
    ui->lineEditPort->setText("6001");
}

void MainWindow::hostNameEdited()
{
    _client.setHostName(ui->lineEditHost->text());
}

void MainWindow::portEdited()
{
    _client.setPort(ui->lineEditPort->text().toInt(NULL,10));
}

void MainWindow::log(const QString &msg)
{
    ui->listWidgetConsole->addItem(msg);
    ui->listWidgetConsole->scrollToBottom();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonQuit_clicked()
{
    this->close();
}


