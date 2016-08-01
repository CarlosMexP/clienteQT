#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QDialog>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE


class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

signals:
    void log(const QString &msg);

public slots:

private slots:
    void send(QString message);
    void sendHello();
    void readServerResponse();
//    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;
};

#endif // TCPCLIENT_H
