#include "broadcaster.h"
#include <QDebug>

Broadcaster::Broadcaster(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    connect(&timer, &QTimer::timeout, this, &Broadcaster::broadcastUdpDatagram);

}

bool Broadcaster::startBroadcaster()
{
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
    QByteArray datagram = "Stupid Bird";
    count++;
    int res = udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, 3456);
}
