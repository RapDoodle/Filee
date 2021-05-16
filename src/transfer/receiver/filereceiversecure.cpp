#include "filereceiversecure.h"

FileReceiverSecure::FileReceiverSecure(SslSocket *sslSocket, QObject *parent)
    : FileReceiver(parent)
{
    setSecureSocket(sslSocket);

    connectSlots();
}

void FileReceiverSecure::connectSlots()
{
    connect(getSecureSocket(), &QTcpSocket::disconnected, this, &FileReceiverSecure::socketConnected);
    connect(getSecureSocket(), &QTcpSocket::readyRead, this, &FileReceiverSecure::readPacket);
    connect(getSecureSocket(), &QSslSocket::encrypted, this, &FileReceiverSecure::onEncrypted);
}

void FileReceiverSecure::onEncrypted()
{
    qDebug() << "EEEEE";
}
