#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDialog>
#include <QListWidgetItem>
#include "tcpclient.h"

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
    void log(const QString &msg);
    void on_pushButtonQuit_clicked();
    void hostNameEdited();
    void portEdited();
private:
    Ui::MainWindow *ui;
    TcpClient _client;
};

#endif // MAINWINDOW_H
