#include "filetransferpeer.h"

FileTransferPeer::FileTransferPeer(QObject *parent) : QObject(parent)
{

}

void FileTransferPeer::sendPacket(PacketType type)
{
    if (!socket)
        return;

    socket->write(reinterpret_cast<const char*>(&type), sizeof(type));
}

void FileTransferPeer::sendPacket(PacketType type, const QByteArray &payload)
{
    if (!socket)
        return;

    qint32 payloadSize = payload.size();
    socket->write(reinterpret_cast<const char*>(&type), sizeof(type));
    socket->write(reinterpret_cast<const char*>(&payloadSize), sizeof(payloadSize));
    socket->write(payload);
}

/* Functions to be overridden */

void FileTransferPeer::pause() {}

void FileTransferPeer::resume() {}

void FileTransferPeer::cancel() {}

void FileTransferPeer::socketBytesWritten() {}

void FileTransferPeer::socketConnected() {}

void FileTransferPeer::socketDisconnected() {}
