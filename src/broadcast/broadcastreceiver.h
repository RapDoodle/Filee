#ifndef BROADCASTRECEIVER_H
#define BROADCASTRECEIVER_H

#include <QObject>
#include <QUdpSocket>

class BroadcastReceiver : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastReceiver(QObject *parent = nullptr);
    bool startBroadcastReceiver();
    bool stopBraodcastReceiver();

private:
    QUdpSocket *udpSocket = nullptr;

signals:
    void broadcastMessageReceived(QHostAddress, QString);

private slots:
    void readUdpPacket();
};

#endif // BROADCASTRECEIVER_H
