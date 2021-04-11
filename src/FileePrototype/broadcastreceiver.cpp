#include "broadcastreceiver.h"

BroadcastReceiver::BroadcastReceiver(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, 6816);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readUdpPacket()));
}

void BroadcastReceiver::readUdpPacket()
{
    uint16_t port;
    QHostAddress sender;
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &port);
        emit broadcastMessageReceived(sender, datagram);
    }
}
