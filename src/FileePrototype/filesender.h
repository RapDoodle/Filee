#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>

#include "filetransferpeer.h"

#define DEFAULT_BUFFER_SIZE 1024

enum class SenderStatus : char
{
    Initialized     = 0x00,
    RequestSent     = 0x01,
    Transferring    = 0x02,
    Completed       = 0x03,
    Paused          = 0x04,
    Canceled        = 0x05,
    Syncing         = 0x06
};

class FileSender : public FileTransferPeer
{
    Q_OBJECT
public:
    explicit FileSender(QString filePath, QHostAddress receiverAddress,
                        qint64 bufferSize, QObject *parent = nullptr);
    ~FileSender();

    void pause() override;
    void resume() override;
    void cancel() override;

private:
    QByteArray fileBuffer;
    qint64 fileBufferSize = -1;
    QString fileDir;

    void sendRequest();
    void sendMeta();
    void sendData();

    SenderStatus status = SenderStatus::Initialized;

signals:
    void transferComplete();
    void transferAborted();

private slots:
    void socketBytesWritten() override;
    void socketConnected() override;
    void socketDisconnected() override;
    void readPacket();

};

#endif // FILESENDER_H
