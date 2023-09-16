#include <QCoreApplication>
#include "udpsender.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UdpSender socket;
    socket.newConnect("127.0.0.1",10100);

    return a.exec();
}
