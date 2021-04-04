#include "broadcastreceiver.h"

BroadcastReceiver::BroadcastReceiver(QObject *parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    bool result = udpSocket->bind(QHostAddress::AnyIPv4, 3456);
    qDebug() << result;
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
        emit broadcastMessageReceived("Message From: " + sender.toString());
        emit broadcastMessageReceived("Port From : " + QString::number(port));
        emit broadcastMessageReceived("Message : " + datagram);
    }
}
