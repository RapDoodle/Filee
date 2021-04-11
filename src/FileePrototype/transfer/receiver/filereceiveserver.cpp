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
        emit receiverInitialized(receiver);
    }
}
