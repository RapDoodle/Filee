#include "broadcaster.h"

Broadcaster::Broadcaster(QString nickname, QObject *parent)
    : QObject(parent)
{
    nameDatagram = nickname.toUtf8();
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(6816, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
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
    udpSocket->writeDatagram(nameDatagram, QHostAddress::Broadcast, 6816);
}
