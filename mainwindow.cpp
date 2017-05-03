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
    connect(ui->pushButtonHello,SIGNAL(clicked(bool)),this,SLOT(sendHello()));
    connect(&_client, SIGNAL(log(QString)), this, SLOT(log(QString)));
    connect(ui->lineEditHost,SIGNAL(textChanged(QString)),this,SLOT(hostNameEdited()));
    connect(ui->lineEditPort,SIGNAL(textChanged(QString)),this,SLOT(portEdited()));
    connect(ui->pushButtonClearConsole,SIGNAL(clicked(bool)),this,SLOT(clearConsole()));
    connect(ui->pushButtonSendMsg,SIGNAL(clicked(bool)),this,SLOT(sendMessage()));

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

void MainWindow::clearConsole()
{
    ui->listWidgetConsole->clear();
}

void MainWindow::sendHello()
{
    QString msg = "Hello";
    _client.send(msg);
    QTime currentTime = QTime::currentTime();
    ui->listWidgetConsole->addItem(QString("[")+currentTime.toString("hh:mm:ss")+"] tx:"+msg);
}


void MainWindow::sendMessage()
{
    QString msg = ui->lineEditSendMsg->text();
    if (msg != NULL){
        _client.send(msg);
        ui->lineEditSendMsg->clear();
        QTime currentTime = QTime::currentTime();
        ui->listWidgetConsole->addItem(QString("[")+currentTime.toString("hh:mm:ss")+"] tx:"+msg);
    }
}

void MainWindow::log(const QString &msg)
{
    QTime currentTime = QTime::currentTime();
    ui->listWidgetConsole->addItem(QString("[")+currentTime.toString("hh:mm:ss")+"] rx:"+msg);
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


