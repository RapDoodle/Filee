#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QStandardPaths>

#include "filetransferpeer.h"

enum class ReceiverStatus : char
{
    Initialized     = 0x00,
    Confirmed       = 0x01,
    Ready           = 0x02,
    Transferring    = 0x03,
    Completed       = 0x04,
    Paused          = 0x05,
    Canceled        = 0x06,
    Error           = 0x07,
    Syncing         = 0x08
};

class FileReceiver : public FileTransferPeer
{
public:
    explicit FileReceiver(QTcpSocket *tcpSocket, QObject *parent = nullptr);

    void pause() override;
    void resume() override;
    void cancel() override;

private:
    void sendData();
    void sendMeta();
    void writeData(QByteArray& data);
    QString fileName;
    ReceiverStatus status = ReceiverStatus::Initialized;
    bool metaProcessed = false;
    void error();
    void overloaded();

signals:

private slots:
    void socketConnected() override;
    void socketDisconnected() override;
    void readPacket();

};

#endif // FILERECEIVER_H
