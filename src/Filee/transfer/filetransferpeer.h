#ifndef FILETRANSFERPEER_H
#define FILETRANSFERPEER_H

#include <QTimer>
#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkProxyFactory>

#include "utils/common.h"

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
    void startRateMeter();
    void stopRateMeter();

protected:
    QTcpSocket* socket = nullptr;
    QTimer rateMeter;
    QFile* file = nullptr;

    QString filename;
    QByteArray socketBuffer;
    qint32 currentPayloadSize = 0;
    qint32 payloadUnreadSize = 0;  // For not finished packet
    qint64 fileSize = 0;
    qint64 sizeProcessed = 0;
    qint64 sizeTransferred = 0;  // For rate meter

    void sendPacket(PacketType type);
    void sendPacket(PacketType type, const QByteArray& payload);

private:
    qint64 lastSizeTransferred = 0;
    int interval = 1000;  // In ms

signals:
    void rateUpdate(QString);

protected slots:
    virtual void socketBytesWritten();
    virtual void socketConnected();
    virtual void socketDisconnected();

};

#endif // FILETRANSFERPEER_H
