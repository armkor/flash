#include <QtWidgets>
#include <QtNetwork>
#include <QPalette>
#include <QColor>

#include "client.h"
#include "ui_client.h"


Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    tcpSocket = new QTcpSocket(this);

    connect(ui->hostlineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableConnectButton()));
    connect(ui->portlineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableConnectButton()));
    connect(ui->connectButton, SIGNAL(clicked()),
            this, SLOT(requestNewConnection()));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readConnection()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

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

        ui->connectButton->setEnabled(false);
        networkSession->open();
    }

}

Client::~Client()
{
    delete ui;
}

void Client::requestNewConnection()
{
    ui->connectButton->setEnabled(false);
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->hostlineEdit->text(),
                             ui->portlineEdit->text().toInt());
}

void Client::readConnection()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_2);

    qint8 type;
    quint16 length;
    qint8 color_r;
    qint8 color_g;
    qint8 color_b;
    in >> type >> length;
    qDebug() << type << length;

    switch ( type ) {
    case 0x12:
      lightOn();
      break;
    case 0x13:
      lightOff();
      break;
    case 0x20:
      in >> color_r >> color_g >> color_b;
      qDebug() << color_r << color_g << color_b;;
      lightColor(QColor(color_r,color_g,color_b));
      break;
    default:
      break;
    }

}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    ui->connectButton->setEnabled(true);
}

void Client::enableConnectButton()
{
    ui->connectButton->setEnabled(!ui->hostlineEdit->text().isEmpty() &&
                                 !ui->portlineEdit->text().isEmpty());

}

void Client::sessionOpened()
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

    enableConnectButton();
}


void Client::lightOn()
{
    ui->flash->setStyleSheet("QLineEdit{background: white;}");
    qDebug() << "Client::lightOn()";
}

void Client::lightOff()
{
    ui->flash->setStyleSheet("QLineEdit{background: black;}");
    qDebug() << "Client::lightOff()";

}

void Client::lightColor(QColor color)
{
    qDebug() << color.red() << color.green() << color.blue();
    ui->flash->setStyleSheet(QString("QLineEdit{background: rgb(%1, %2,%3);}")
                             .arg(color.red()).arg(color.green()).arg(color.blue()));
    qDebug() << "Client::lightColor()";
}
