#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QtNetwork/QUdpSocket>
#include <QThread>


class UdpSender : public QUdpSocket
{
    Q_OBJECT

public:
//    QUdpSocket *socket = Q_NULLPTR;
    explicit UdpSender(QObject *parent = nullptr);
    void newConnect(QString address, int port);
    QByteArray packedStruct();
};


struct DataStruct
{
    int hour, min, sec; // время
    double fuelWeight;  // вес топлива, кг
    union {
        unsigned int word; // слово данных
        struct {
            unsigned char addr : 8;     // адрес (в 16-тиричном представлении)
            unsigned char reserve1 : 6; // какой-то резерв, не нужно для использования
            bool dataOk : 1;            // признак корректности данных
            bool onLand : 1;            // признак нахождения на земле
            unsigned int speed : 10;    // скорость, км/ч (цена старшего разряда - 512)
            unsigned char reserve2 : 3; // какой-то резерв, не нужно для использования
            unsigned char matrix : 2;   // матрица состояния(00 - отказ, 01 – нет данных, 10 – тест, 11 - нормальная работа)
            bool ctrlBit : 1;           // бит контроля чётности
        } data;
    } dataUnion;
    bool isCrashed;     // признак разрушения
}__attribute__((packed));

#endif // UDPSENDER_H
