#ifndef FILETRANSFERPEER_H
#define FILETRANSFERPEER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

enum class PacketType : char
{
    Data        = 0x00,
    Request     = 0x01,
    Accepted    = 0x02,
    Deny        = 0x03,
    Meta        = 0x04,
    Complete    = 0x05,
    Pause       = 0x06,
    Resume      = 0x07,
    Cancel      = 0x08
};

class FileTransferPeer : public QObject
{
    Q_OBJECT
public:
    explicit FileTransferPeer(QObject *parent = nullptr);

    virtual void pause();
    virtual void resume();
    virtual void cancel();

protected:
    QTcpSocket* socket = nullptr;
    QFile* file = nullptr;
    QByteArray socketBuffer;
    qint32 currentPayloadSize = 0;
    qint32 payloadUnreadSize = 0;  // For not finished packet
    qint64 fileSize = -1;
    qint64 sizeProcessed = -1;

    void activateSocket();
    void sendPacket(PacketType type);
    void sendPacket(PacketType type, const QByteArray& payload);

signals:

protected slots:
    virtual void socketBytesWritten();
    virtual void socketConnected();
    virtual void socketDisconnected();

};

#endif // FILETRANSFERPEER_H
