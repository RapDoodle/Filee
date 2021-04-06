#include "filesender.h"

FileSender::FileSender(QString filePath, QHostAddress receiverAddress, QObject *parent)
    : FileTransferPeer(parent), fileDir(filePath)
{
    fileBufferSize = BUFFER_SIZE;
//    fileBuffer.resize(fileBufferSize);
    fileBuffer.resize(1024);

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

    activateSocket();
    connect(socket, &QTcpSocket::readyRead, this, &FileSender::readPacket);

    // Send meta info

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

//    if (fileSize - sizeProcessed < fileBufferSize) {
    if (fileSize - sizeProcessed < 1024) {
        fileBufferSize = fileSize - sizeProcessed;
        fileBuffer.resize(fileBufferSize);
    }

//    qint64 bytesRead = file->read(fileBuffer.data(), fileBufferSize);
    qint64 bytesRead = file->read(fileBuffer.data(), 1024);
//    qDebug() << bytesRead;
    if (bytesRead == -1)
        return;

    sizeProcessed += bytesRead;

    // Emit progress change
    emit statusUpdate((int)((double)sizeProcessed * 100 / fileSize));

    sendPacket(PacketType::Data, fileBuffer);

    if (fileSize - sizeProcessed <= 0) {
        sizeProcessed = fileSize;
        file->close();
        fileBuffer.clear();
        sendPacket(PacketType::Complete);
    }
}

void FileSender::readPacket()
{
    QByteArray buffer;
    buffer.append(socket->readAll());

    while (buffer.size() > 0) {
        qDebug() << buffer;
        PacketType type = static_cast<PacketType>(buffer.at(0));
        qint32 payloadSize = 0;

        switch (type) {
            case PacketType::Accepted: {
                status = SenderStatus::Transferring;
                sendMeta();
//                sendData();
                break;
            }
            default: {
                qDebug() << "Sender: Default case";
                break;
            }
        }

        if (payloadSize == 0) {
            buffer.remove(0, sizeof(type));
        } else {
            buffer.remove(0, sizeof(type) + sizeof(qint32) + payloadSize);
        }

    }
}

void FileSender::socketBytesWritten()
{
    if (status == SenderStatus::Transferring)
        sendData();
}

void FileSender::socketConnected()
{
    sendPacket(PacketType::Request);
}

void FileSender::socketDisconnected()
{
    qDebug() << "Disconnected.";
}

void FileSender::pause()
{

}

void FileSender::resume()
{

}

void FileSender::cancel()
{

}
