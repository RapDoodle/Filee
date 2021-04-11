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
#include <QNetworkProxyFactory>

enum class PacketType : char
{
    Data        = 0x00,
    RequestData = 0x01,
    SendRequest = 0x02,
    KeepAlive   = 0x03,  // To be implemented
    Accept      = 0x04,
    Deny        = 0x05,
    Complete    = 0x06,
    Pause       = 0x07,
    Resume      = 0x08,
    Cancel      = 0x09,
    Error       = 0x0A,
    SyncRequest = 0x0B,  // Sent by receiver on overload
    ConfirmSync = 0x0C,  // Sent by sender on overload
    SyncDone    = 0x0D
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

    void sendPacket(PacketType type);
    void sendPacket(PacketType type, const QByteArray& payload);

signals:
    void statusUpdate(int);

protected slots:
    virtual void socketBytesWritten();
    virtual void socketConnected();
    virtual void socketDisconnected();

};

#endif // FILETRANSFERPEER_H
