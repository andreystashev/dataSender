#include "udpsender.h"
#include <QRandomGenerator>
#include <QTime>


UdpSender::UdpSender(QObject *parent)
    : QUdpSocket{parent}
{

}

void UdpSender::newConnect(QString address, int port)
{
    QHostAddress hostAddress(address);
    int counter = 0;
    while(1) {
        QByteArray datagram = packedStruct();
        writeDatagram(datagram, hostAddress, port);
        QThread::msleep(50);
        qDebug()<<"send:"<<datagram.toHex()<<datagram.size()<<counter;
        ++counter;
    }
}

QByteArray UdpSender::packedStruct(){
    QTime currentTime = QTime::currentTime();
    DataStruct data;
    static double fuel = 1000.0;
    data.hour=currentTime.hour();
    data.min=currentTime.minute();
    data.sec=currentTime.second();
    data.fuelWeight = fuel;
    fuel-=0.001;
    data.dataUnion.data.addr=QRandomGenerator::global()->bounded(0, 255);
    data.dataUnion.data.reserve1=QRandomGenerator::global()->bounded(0, 255);
    data.dataUnion.data.dataOk=QRandomGenerator::global()->bounded(0, 2);
    data.dataUnion.data.onLand=QRandomGenerator::global()->bounded(0, 2);
    data.dataUnion.data.speed=QRandomGenerator::global()->bounded(1, 5);
    data.dataUnion.data.reserve2=QRandomGenerator::global()->bounded(0, 255);
    data.dataUnion.data.matrix=QRandomGenerator::global()->bounded(0, 4);
    data.dataUnion.data.ctrlBit=QRandomGenerator::global()->bounded(0, 2);
    data.isCrashed=QRandomGenerator::global()->bounded(0, 2);

    QByteArray array;
    array.append(reinterpret_cast<const char*>(&data.hour),sizeof(data.hour));
    array.append(reinterpret_cast<const char*>(&data.min),sizeof(data.min));
    array.append(reinterpret_cast<const char*>(&data.sec),sizeof(data.sec));
    array.append(reinterpret_cast<const char*>(&data.fuelWeight),sizeof(data.fuelWeight));
    array.append(reinterpret_cast<const char*>(&data.dataUnion.word),sizeof(data.dataUnion.word));
    array.append(reinterpret_cast<const char*>(&data.dataUnion.data),sizeof(data.dataUnion.data));
    array.append(reinterpret_cast<const char*>(&data.isCrashed),sizeof(data.isCrashed));

    return array;
}

