#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class Broadcaster : public QObject
{
    Q_OBJECT
public:
    explicit Broadcaster(QString, QObject *parent = nullptr);
    Q_INVOKABLE bool startBroadcaster();
    Q_INVOKABLE bool stopBroadcaster();

private:
    QByteArray nameDatagram;
    QUdpSocket *udpSocket = nullptr;
    QTimer timer;

private slots:
    void broadcastUdpDatagram();

signals:

};

#endif // BROADCASTER_H
