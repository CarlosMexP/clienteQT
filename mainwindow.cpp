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
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readServerResponse()));
//! [2] //! [4]
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//! [3]
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    connect(ui->pushButtonHello,SIGNAL(clicked(bool)),this,SLOT(sendHello()));

    ui->pushButtonHello->setEnabled(true);
    ui->lineEditHost->setText("192.168.0.27");
    ui->lineEditPuerto->setText("6001");

/*    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        ui->listWidgetHost->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            ui->listWidgetHost->addItem(name + QChar('.') + domain);
    }
    if (name != QString("localhost"))
        ui->listWidgetHost->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback() &&  ipAddressesList.at(i).toString().contains("."))
            ui->listWidgetHost->addItem(ipAddressesList.at(i).toString());

    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback() && ipAddressesList.at(i).toString().contains("."))
            ui->listWidgetHost->addItem(ipAddressesList.at(i).toString());
    }

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        //statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
*/
}



void MainWindow::send(QString message)
{
    tcpSocket->abort();
    QString hostName = ui->lineEditHost->text();
    int port = ui->lineEditPuerto->text().toInt(NULL,10);
    tcpSocket->connectToHost(hostName,port);
    qDebug() << "CONNECTED";
    int numBytes = tcpSocket->write(QByteArray::fromStdString(message.toStdString()));
    qDebug() << "Num de written bytes: "<< numBytes;
}

void MainWindow::sendHello()
{
    send("HELLO");
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Warning"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Warning"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Warning"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

}


void MainWindow::enableButtons()
{
    if (!networkSession || networkSession->isOpen())
    {
        ui->pushButtonHello->setEnabled(true);
    }
}

void MainWindow::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
    enableButtons();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonQuit_clicked()
{
    this->close();
}

void MainWindow::readServerResponse()
{
    QByteArray trama = tcpSocket->readAll();
    QString response;
    response.append(trama);
    //Divide una cadena en líneas
    QStringList lines = response.split( "\n", QString::SkipEmptyParts );
    /*ui->listWidgetConsole->clear();
    foreach (response, lines) {
        response =response.trimmed();
        qDebug() << response;
            ui->listWidgetConsole->addItem(response);
    }*/
    ui->listWidgetConsole->addItem(trama);
    ui->listWidgetConsole->scrollToBottom();
}


void MainWindow::on_listWidgetConsole_itemClicked(QListWidgetItem *item)
{
}
