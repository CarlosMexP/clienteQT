#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QtNetwork>

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

private:
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;
};

#endif // TCPCLIENT_H
