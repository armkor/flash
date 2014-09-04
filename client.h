#ifndef CLIENT_H
#define CLIENT_H

#include <QNetworkSession>
#include <QTcpSocket>
#include <QWidget>
#include <QDebug>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void requestNewConnection();
    void readConnection();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableConnectButton();
    void sessionOpened();

    void lightOn();
    void lightOff();
    void lightColor(QColor color);

private:
    Ui::Client *ui;
    QTcpSocket *tcpSocket;
    quint16 blockSize;

    QNetworkSession *networkSession;
};

#endif // CLIENT_H
