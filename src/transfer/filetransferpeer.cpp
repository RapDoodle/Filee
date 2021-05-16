#include "filetransferpeer.h"

FileTransferPeer::FileTransferPeer(QObject *parent) : QObject(parent)
{
    connect(&rateMeter, &QTimer::timeout, this, [&]() {
        emit rateUpdate(Common::humanReadableSize(sizeTransferred - lastSizeTransferred) + "/s");
        lastSizeTransferred = sizeTransferred;
    });
}

void FileTransferPeer::sendPacket(PacketType type)
{

    if (useSecure) {
        if (secureSocket)
            secureSocket->write(reinterpret_cast<const char*>(&type), sizeof(type));
    } else {
        if(socket)
            socket->write(reinterpret_cast<const char*>(&type), sizeof(type));
    }
}

void FileTransferPeer::sendPacket(PacketType type, const QByteArray &payload)
{
    qint32 payloadSize = payload.size();

    if (useSecure) {
        if (secureSocket) {
            secureSocket->write(reinterpret_cast<const char*>(&type), sizeof(type));
            secureSocket->write(reinterpret_cast<const char*>(&payloadSize), sizeof(payloadSize));
            secureSocket->write(payload);
        }
    } else {
        if (socket) {
            socket->write(reinterpret_cast<const char*>(&type), sizeof(type));
            socket->write(reinterpret_cast<const char*>(&payloadSize), sizeof(payloadSize));
            socket->write(payload);
        }
    }
}

void FileTransferPeer::closeSocket() {
    if (useSecure) {
        if (secureSocket) {
            secureSocket->close();
            secureSocket = nullptr;
        }
    } else {
        if (socket) {
            socket->close();
            socket = nullptr;
        }
    }
}

QHostAddress FileTransferPeer::getPeerAddress() {
    if (useSecure) {
        if (secureSocket)
            return secureSocket->peerAddress();
    } else {
        if (socket)
            return socket->peerAddress();
    }
    return QHostAddress::Null;
}

qint64 FileTransferPeer::getBytesAvailable() {
    if (useSecure) {
        if (secureSocket)
            return secureSocket->bytesAvailable();
    } else {
        if (socket)
            return socket->bytesAvailable();
    }
    return 0;
}

QByteArray FileTransferPeer::readSocketBuffer() {
    if (useSecure) {
        if (secureSocket)
            return secureSocket->readAll();
    } else {
        if (socket)
            return socket->readAll();
    }
    return QByteArray();
}

QTcpSocket* FileTransferPeer::getSocket() { return socket; }

SslSocket* FileTransferPeer::getSecureSocket() { return secureSocket; }

void FileTransferPeer::setSocket(QTcpSocket* newSocket) { socket = newSocket; useSecure = false; }

void FileTransferPeer::setSecureSocket(SslSocket* newSocket) { secureSocket = newSocket; useSecure = true; }

void FileTransferPeer::startRateMeter() { rateMeter.start(interval); }

void FileTransferPeer::stopRateMeter() { rateMeter.stop(); }

/* Functions to be overridden */

void FileTransferPeer::pause() {}

void FileTransferPeer::resume() {}

void FileTransferPeer::cancel() {}

void FileTransferPeer::socketBytesWritten() {}

void FileTransferPeer::socketConnected() {}

void FileTransferPeer::socketDisconnected() {}
