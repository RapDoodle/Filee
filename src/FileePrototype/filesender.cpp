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
    sendPacket(PacketType::Request);
}

void FileSender::sendMeta() {
    QString fileName = QDir(file->fileName()).dirName();
    QJsonObject obj(QJsonObject::
                    fromVariantMap(
                        {
                            {"name", fileName},
                            {"size", fileSize},
                        }));
    QByteArray payload(QJsonDocument(obj).toJson());
    sendPacket(PacketType::Meta, payload);
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
            case PacketType::Accepted: {
                status = SenderStatus::Transferring;
                sendMeta();
//                sendData();
                break;
            }
            case PacketType::ACK: {
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
    sendPacket(PacketType::Request);
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
