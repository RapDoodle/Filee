#ifndef FILESENDER_H
#define FILESENDER_H

#include "./transfer/filetransferpeer.h"

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
    void getFileName();

private:
    QByteArray fileBuffer;
    qint64 fileBufferSize = -1;
    QString fileDir;

    SenderStatus status = SenderStatus::Initialized;

    void sendRequest();
    void sendData();

signals:
    void senderBegin();
    void senderEnded();
    void senderStatusUpdate(int);

    void restartRequest();

private slots:
    void socketBytesWritten() override;
    void socketConnected() override;
    void socketDisconnected() override;
    void readPacket();

};

#endif // FILESENDER_H
