#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork>
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
    ui->pushButtonVerPaneles->setDefault(true);
    ui->pushButtonVerPaneles->setEnabled(true);
    ui->pushButtonAbrePuerta->setEnabled(false);
    ui->pushButtonCierraPuerta->setEnabled(false);
    ui->lineEditHost->setText("192.168.1.14");
    ui->lineEditPuerto->setText("6010");

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

        ui->pushButtonVerPaneles->setEnabled(false);
        //statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
*/
}



void MainWindow::solicitaListaPaneles()
{
    ui->pushButtonVerPaneles->setEnabled(false);
    blockSize = 0;
    tcpSocket->abort();
//! [7]
    tcpSocket->connectToHost("192.168.0.158",6010);
    qDebug() << "CONECTANDO";
    int numBytes = tcpSocket->write("LISTA DE PANELES");
 //   tcpSocket->flush();
    qDebug() << "Num de bytes escritos "<< numBytes;
//! [7]
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Atención"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Atención"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Atención"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    ui->pushButtonVerPaneles->setEnabled(true);
}
void MainWindow::readFortune()
{
//! [9]
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
//! [8]

//! [10]
        in >> blockSize;
    }

    if (tcpSocket->bytesAvailable() < blockSize)
        return;
//! [10] //! [11]

    QString nextFortune;
    in >> nextFortune;

    if (nextFortune == currentFortune) {
        QTimer::singleShot(0, this, SLOT(solicitaListaPaneles()));
        return;
    }
//! [11]

//! [12]
    currentFortune = nextFortune;
//! [9]
    ui->listWidgetPaneles->addItem(currentFortune);
    ui->pushButtonVerPaneles->setEnabled(true);


}

void MainWindow::enableGetFortuneButton()
{
    ui->pushButtonVerPaneles->setEnabled((!networkSession || networkSession->isOpen()));

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

 //   statusLabel->setText(tr("This examples requires that you run the "
                //            "Fortune Server example as well."));

    enableGetFortuneButton();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonVerPaneles_clicked()
{
    this->solicitaListaPaneles();
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
    ui->listWidgetPaneles->clear();
    foreach (response, lines) {
        response =response.trimmed();
        qDebug() << response;
            ui->listWidgetPaneles->addItem(response);
    }
}

void MainWindow::on_pushButtonAbrePuerta_clicked()
{
     tcpSocket->abort();
    tcpSocket->connectToHost("192.168.0.158",6010);
    qDebug() << "ENVIANDO COMANDO ABRE";
    QString nombrePanelSeleccionado =
            ui->listWidgetPaneles->selectedItems().first()->text();
    //nombrePanelSeleccionado
    QByteArray nPanelSel;
    nPanelSel.append("ABRE,"+nombrePanelSeleccionado);
    int numBytes = tcpSocket->write((const char *) nPanelSel);
    qDebug() << "abriendo Puerta en Panel "<< nPanelSel;
    tcpSocket->close();

}

void MainWindow::on_pushButtonCierraPuerta_clicked()
{
    tcpSocket->abort();
   tcpSocket->connectToHost("192.168.0.158",6010);
   qDebug() << "ENVIANDO COMANDO CIERRA";
   QString nombrePanelSeleccionado =
           ui->listWidgetPaneles->selectedItems().first()->text();
   //nombrePanelSeleccionado
   QByteArray nPanelSel;
   nPanelSel.append("CIERRA,"+nombrePanelSeleccionado);
   int numBytes = tcpSocket->write((const char *) nPanelSel);
   qDebug() << "abriendo Puerta en Panel "<< nPanelSel;
   tcpSocket->close();
}

void MainWindow::on_listWidgetPaneles_itemClicked(QListWidgetItem *item)
{
    ui->pushButtonAbrePuerta->setEnabled(true);
    ui->pushButtonCierraPuerta->setEnabled(true);
}
