#include "filereceiveserver.h"

FileReceiveServer::FileReceiveServer(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::AnyIPv4, 3800);
    connect(tcpServer, &QTcpServer::newConnection, this, &FileReceiveServer::newConnection);
}

void FileReceiveServer::newConnection()
{
    QTcpSocket* socket = tcpServer->nextPendingConnection();
    if (socket) {
        FileReceiver* receiver = new FileReceiver(socket);
        receivers.push_back(receiver);
        connect(receiver, &FileReceiver::receiverBegin, this, [&](QString sender, QString filename, QString filePath) {
            emit receiverBegin(sender, filename, filePath);
        });
        connect(receiver, &FileReceiver::receiverEnded, this, [&]() {
            emit receiverEnded();
        });
        connect(receiver, QOverload<int>::of(&FileReceiver::receiverStatusUpdate),
                this, [&](int status) {
            emit receiverStatusUpdate(status);
        });
        connect(receiver, QOverload<QString>::of(&FileReceiver::rateUpdate),
                this, [&](QString rate) {
            emit rateUpdate(rate);
        });
    }
}
