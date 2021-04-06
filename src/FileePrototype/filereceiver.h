#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QProcessEnvironment>

#include "filetransferpeer.h"

enum class ReceiverStatus : char
{
    Initialized     = 0x00,
    Confirmed       = 0x01,
    Ready           = 0x02,
    Transferring    = 0x03,
    Completed       = 0x04,
    Paused          = 0x05,
    Cancelled       = 0x06
};

class FileReceiver : public FileTransferPeer
{
public:
    explicit FileReceiver(QTcpSocket *tcpSocket, QObject *parent = nullptr);

private:
    void sendData();
    void sendMeta();
//    void metaParser(QByteArray& data);
    void writeData(QByteArray& data);
    ReceiverStatus status = ReceiverStatus::Initialized;

    QString fileName;



signals:

private slots:
    void socketConnected() override;
    void socketDisconnected() override;
    void readPacket();

};

#endif // FILERECEIVER_H
