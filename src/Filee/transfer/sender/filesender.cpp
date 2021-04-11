#include "filesender.h"

FileSender::FileSender(QString filePath, QHostAddress receiverAddress, qint64 bufferSize, QObject *parent)
    : FileTransferPeer(parent), fileBufferSize(bufferSize), fileDir(filePath)
{
    fileBuffer.resize(fileBufferSize);

    file = new QFile(fileDir, this);
    bool opened = file->open(QIODevice::ReadOnly);

    if (!opened)
        return;

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

    // Send meta info

}

FileSender::~FileSender()
{
    qDebug() << "FileSender destcutor called";
    disconnect(socket, &QTcpSocket::bytesWritten, this, &FileSender::socketBytesWritten);
    disconnect(socket, &QTcpSocket::connected, this, &FileSender::socketConnected);
    disconnect(socket, &QTcpSocket::disconnected, this, &FileSender::socketDisconnected);
    disconnect(socket, &QTcpSocket::readyRead, this, &FileSender::readPacket);
}


void FileSender::sendRequest()
{
    QString fileName = QDir(file->fileName()).dirName();
    #if defined (Q_OS_ANDROID)
    fileName = Utils::androidFileNameParser(file->fileName());
    #endif
    QJsonObject obj(QJsonObject::
                    fromVariantMap(
                        {
                            {"name", fileName},
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
    emit statusUpdate((int)((double)sizeProcessed * 10000 / fileSize));

    sendPacket(PacketType::Data, fileBuffer);

    if (fileSize - sizeProcessed <= 0) {
        sizeProcessed = fileSize;
        file->close();
        fileBuffer.clear();
        status = SenderStatus::Completed;
        sendPacket(PacketType::Complete);
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
                break;
            }
            case PacketType::RequestData: {
                sendData();
                break;
            }
            case PacketType::Deny: {
                // Denied by receiver
                break;
            }
            case PacketType::Pause: {
                qDebug() << "[Sender] Pause";
                pause();
                break;
            }
            case PacketType::Resume: {
                qDebug() << "[Sender] Resume";
                resume();
                break;
            }
            case PacketType::Cancel: {
                qDebug() << "[Sender] Cancel";
                if (status == SenderStatus::Transferring || status == SenderStatus::Paused)
                    status = SenderStatus::Canceled;
                emit statusUpdate(0);
                break;
            }
            case PacketType::Error: {
                qDebug() << "Error signal received";
                socket->close();
                file->close();
                emit transferAborted();
                return;
            }
            case PacketType::SyncRequest: {
                qDebug() << "[Sender] Request to sync received";
                status = SenderStatus::Syncing;
                try {
                    memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                } catch (...) {
                    return;
                }
                if (payloadSize + (qint64)(sizeof(type) + sizeof(qint32)) > socketBuffer.size()) {
//                    sendPacket(PacketType::RequestData);
                    return;
                }
                qint64 pos = 0;
                memcpy(&pos, socketBuffer.mid(sizeof(type) + sizeof(qint32), sizeof(qint64)), sizeof(qint64));
                qDebug() << "[Sender] Received SYNC to position " << pos;
                sizeProcessed = pos;
                file->seek(sizeProcessed);
                // Confirm the position
                pos = file->pos();
                QByteArray packet(reinterpret_cast<const char *>(&pos), sizeof(pos));
                sendPacket(PacketType::ConfirmSync, packet);
                break;
            }
            case PacketType::SyncDone: {
                qDebug() << "Sync done";
                status = SenderStatus::Transferring;
                break;
            }
            default: {
                qDebug() << "[Sender] Default case. Could be a serious error!" << socketBuffer.constData();
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

void FileSender::socketBytesWritten()
{

}

void FileSender::socketConnected()
{
    sendRequest();
}

void FileSender::socketDisconnected()
{
    // Cancel when the receiver has disconnected.
    status = SenderStatus::Canceled;
    qDebug() << "Disconnected.";
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
    if (status == SenderStatus::Transferring || status == SenderStatus::Paused)
        status = SenderStatus::Canceled;
    emit statusUpdate(0);
    sendPacket(PacketType::Cancel);
}
