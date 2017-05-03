#include "tcpclient.h"
#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readServerResponse()));
//    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void TcpClient::setHostName(QString hostName)
{
    _hostName = hostName;
}

void TcpClient::setPort(int port)
{
    _port = port;
}


void TcpClient::send(QString message)
{
    tcpSocket->abort();
    tcpSocket->connectToHost(_hostName,_port);
    qDebug() << "CONNECTED";
    int numBytes = tcpSocket->write(QByteArray::fromStdString(message.toStdString()));
    qDebug() << "Num de written bytes: "<< numBytes;
}

void TcpClient::readServerResponse()
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
    emit log(trama);
}

/*void TcpClient::displayError(QAbstractSocket::SocketError socketError)
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

}*/
