#include "filesender.h"

FileSender::FileSender(QString filePath, QHostAddress receiverAddress, qint64 bufferSize, QObject *parent)
    : FileTransferPeer(parent), fileBufferSize(bufferSize), fileDir(filePath)
{
    fileBuffer.resize(fileBufferSize);

    file = new QFile(fileDir, this);
    bool opened = file->open(QIODevice::ReadOnly);

    if (!opened) {
        MessageBox::messageBoxCritical("Unable to open the file to read.");
        return;
    }

    if (file->size() == 0)
        return;

    fileSize = file->size();
    sizeProcessed = 0;

    socket = new QTcpSocket(this);
    socket->setProxy(QNetworkProxy::NoProxy);
    socket->connectToHost(receiverAddress, 3800, QAbstractSocket::ReadWrite);

    connect(socket, &QTcpSocket::bytesWritten, this, &FileSender::socketBytesWritten);
    connect(socket, &QTcpSocket::connected, this, &FileSender::socketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &FileSender::socketDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &FileSender::readPacket);

}

FileSender::~FileSender()
{
    disconnect(socket, &QTcpSocket::bytesWritten, this, &FileSender::socketBytesWritten);
    disconnect(socket, &QTcpSocket::connected, this, &FileSender::socketConnected);
    disconnect(socket, &QTcpSocket::disconnected, this, &FileSender::socketDisconnected);
    disconnect(socket, &QTcpSocket::readyRead, this, &FileSender::readPacket);
}


void FileSender::sendRequest()
{
#if defined (Q_OS_ANDROID)
    filename = AndroidUtils::androidFileNameParser(file->fileName());
#else
    filename = QDir(file->fileName()).dirName();
#endif
    QJsonObject obj(QJsonObject::
                    fromVariantMap(
                        {
                            {"name", filename},
                            {"size", fileSize},
                        }));
    QByteArray payload(QJsonDocument(obj).toJson());
    sendPacket(PacketType::SendRequest, payload);
}

void FileSender::sendData()
{
    if (status != SenderStatus::Transferring)
        return;

    if (fileSize - sizeProcessed < fileBufferSize) {
        fileBufferSize = fileSize - sizeProcessed;
        fileBuffer.resize(fileBufferSize);
    }

    qint64 bytesRead = file->read(fileBuffer.data(), fileBufferSize);
    if (bytesRead == -1)
        return;

    sizeProcessed += bytesRead;

    // Emit progress change
    emit senderStatusUpdate((int)((double)sizeProcessed * 10000 / fileSize));

    sendPacket(PacketType::Data, fileBuffer);

    if (fileSize - sizeProcessed <= 0) {
        sizeProcessed = fileSize;
        file->close();
        fileBuffer.clear();
        status = SenderStatus::Completed;
        sendPacket(PacketType::Complete);
        emit senderEnded();
        MessageBox::messageBoxInfo("The transfer has completed.");
    }
}

void FileSender::readPacket()
{
    socketBuffer.append(socket->readAll());

    while (socketBuffer.size() > 0) {
        PacketType type = static_cast<PacketType>(socketBuffer.at(0));
        qint32 payloadSize = 0;

        switch (type) {
            case PacketType::Accept: {
                status = SenderStatus::Transferring;
                startRateMeter();
                emit senderBegin();
                break;
            }
            case PacketType::RequestData: {
                sendData();
                break;
            }
            case PacketType::Deny: {
                // Denied by receiver
                socket->close();
                file->close();
                MessageBox::messageBoxCritical("The transfer was rejected by the remote peer.");
                break;
            }
            case PacketType::Pause: {
                pause();
                break;
            }
            case PacketType::Resume: {
                resume();
                break;
            }
            case PacketType::Cancel: {
                // Do not notify the sender of the termination if it was done by the sender
                if (status != SenderStatus::Canceled)
                    MessageBox::messageBoxCritical("The transfer was terminated by the remote peer.");
                if (status == SenderStatus::Transferring || status == SenderStatus::Paused)
                    status = SenderStatus::Canceled;
                emit senderStatusUpdate(0);
                emit senderEnded();
                stopRateMeter();
                break;
            }
            case PacketType::Error: {
                socket->close();
                file->close();
                emit restartRequest();
                stopRateMeter();
                return;
            }
            case PacketType::SyncRequest: {
                status = SenderStatus::Syncing;
                try {
                    memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                } catch (...) {
                    return;
                }
                if (payloadSize + (qint64)(sizeof(type) + sizeof(qint32)) > socketBuffer.size())
                    return;
                qint64 pos = 0;
                memcpy(&pos, socketBuffer.mid(sizeof(type) + sizeof(qint32), sizeof(qint64)), sizeof(qint64));
                sizeProcessed = pos;
                file->seek(sizeProcessed);
                // Confirm the position
                pos = file->pos();
                QByteArray packet(reinterpret_cast<const char *>(&pos), sizeof(pos));
                sendPacket(PacketType::ConfirmSync, packet);
                break;
            }
            case PacketType::SyncDone: {
                status = SenderStatus::Transferring;
                break;
            }
            default: {
                // Abort on error packet
                cancel();
                stopRateMeter();
                break;
            }
        }

        if (payloadSize == 0) {
            socketBuffer.remove(0, sizeof(type));
        } else {
            socketBuffer.remove(0, sizeof(type) + sizeof(qint32) + payloadSize);
        }

    }
}

void FileSender::socketBytesWritten() { sizeTransferred = sizeProcessed; }

void FileSender::socketConnected() { sendRequest(); }

void FileSender::socketDisconnected()
{
    // Cancel when the receiver has disconnected.
    status = SenderStatus::Canceled;
    stopRateMeter();
}

void FileSender::pause()
{
    if (status == SenderStatus::Transferring)
        status = SenderStatus::Paused;
}

void FileSender::resume()
{
    if (status == SenderStatus::Paused) {
        status = SenderStatus::Transferring;
        sendData();
    }
}

void FileSender::cancel()
{
    if (status == SenderStatus::Transferring || status == SenderStatus::Paused) {
        status = SenderStatus::Canceled;
        emit senderStatusUpdate(0);
        sendPacket(PacketType::Cancel);
    }
    stopRateMeter();
}
