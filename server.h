#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QDebug>
#include <QSpinBox>

class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);

private slots:
    void sessionOpened();
    //void sendFortune(qint8 type = 0x12, quint16 length = 0, bool isColor = false);
    //void sendFortune(qint8 type = 0x13, quint16 length = 0, bool isColor = false);
    void sendFortune(qint8 type = 0x20, quint16 length = 3, bool isColor = true);
    //void sendFortune(qint8 type = 0x16, quint16 length = 0, bool isColor = false);

private:
    QLabel *statusLabel;
    QSpinBox *color_r;
    QSpinBox *color_g;
    QSpinBox *color_b;

    QPushButton *quitButton;
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
};

#endif
