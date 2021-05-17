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
    // Code removed since in Qt5, all send requests are queued.
    // Any message "send" while the handshake is not complete
    // will be stored in the buffer.
}
