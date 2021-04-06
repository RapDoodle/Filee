#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include "filetransferpeer.h"

class FileReceiver : public FileTransferPeer
{
public:
    explicit FileReceiver(QTcpSocket *tcpSocket, QObject *parent = nullptr);

private:
    void sendData();
    void sendMeta();
    void metaParser(QByteArray& data);
    void writeData(QByteArray& data);

    QString fileName;



signals:

private slots:
    void socketConnected() override;
    void socketDisconnected() override;
    void readPacket();

};

#endif // FILERECEIVER_H
