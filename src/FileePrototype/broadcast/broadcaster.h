#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class Broadcaster : public QObject
{
    Q_OBJECT
public:
    explicit Broadcaster(QObject *parent = nullptr);
    bool startBroadcaster();
    bool stopBroadcaster();

private:
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;
    int count = 0;

private slots:
    void broadcastUdpDatagram();

signals:

};

#endif // BROADCASTER_H
