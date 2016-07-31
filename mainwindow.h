#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDialog>
#include <QListWidgetItem>
QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void solicitaListaPaneles();
    void on_pushButtonVerPaneles_clicked();
    void readFortune();
    void readServerResponse();
    void enableGetFortuneButton();
    void sessionOpened();
    void displayError(QAbstractSocket::SocketError socketError);

    void on_pushButtonQuit_clicked();

    void on_pushButtonAbrePuerta_clicked();

    void on_pushButtonCierraPuerta_clicked();

    void on_listWidgetPaneles_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
    QString currentFortune;
    quint16 blockSize;

    QNetworkSession *networkSession;
};

#endif // MAINWINDOW_H
