#include "client.h"
#include "server.h"
#include <QApplication>
#include <QTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client w;
    w.show();

    Server server;
    server.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return a.exec();
}
