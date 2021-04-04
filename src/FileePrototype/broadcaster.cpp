#include "broadcaster.h"
#include <QDebug>

Broadcaster::Broadcaster(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(&timer, &QTimer::timeout, this, &Broadcaster::broadcastUdpDatagram);

}

bool Broadcaster::startBroadcaster()
{
    qDebug() << "Running";
    timer.start(1000);
    return true;
}

bool Broadcaster::stopBroadcaster()
{
    timer.stop();
    return true;
}

void Broadcaster::broadcastUdpDatagram()
{
    QByteArray datagram = "Hi NO." + QByteArray::number(count);
    count++;
    int res = udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 3456);
    qDebug() << res;
}
