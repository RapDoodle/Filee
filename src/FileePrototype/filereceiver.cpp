#include "filereceiver.h"

FileReceiver::FileReceiver(QTcpSocket *tcpSocket, QObject *parent)
    : FileTransferPeer(parent)
{
    socket = tcpSocket;

    connect(socket, &QTcpSocket::disconnected, this, &FileReceiver::socketConnected);
    connect(socket, &QTcpSocket::readyRead, this, &FileReceiver::readPacket);
}

void FileReceiver::socketConnected()
{

}

void FileReceiver::socketDisconnected()
{
    if (socket)
        socket->close();
}

void FileReceiver::readPacket()
{
    socketBuffer.append(socket->readAll());

    while (socketBuffer.size() > 0) {
        PacketType type = static_cast<PacketType>(socketBuffer.at(0));
        qint32 payloadSize = 0;

        switch (type) {
            case PacketType::Data: {
                if (status == ReceiverStatus::Error)
                    return;
                if (status != ReceiverStatus::Transferring)
                    return;
                memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                if (payloadSize > (qint32)socketBuffer.size() + (qint32)(sizeof(type) + sizeof(sizeof(qint32)))) {
                    sendPacket(PacketType::ACK);
                    return;
                }
                QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                writeData(data);
                break;
            }
            case PacketType::Request: {
                qDebug() << "Type: request";
                sendPacket(PacketType::Accepted);
                break;
            }
            case PacketType::Meta: {
                if (metaProcessed)
                    return error();
                qDebug() << "Type: meta";
                payloadSize = socketBuffer.at(1);
                QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                QJsonObject obj = QJsonDocument::fromJson(data).object();
                fileSize = obj.value("size").toVariant().value<qint64>();
                fileName = obj.value("name").toString();

                QDir dir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/Filee");
                if (!dir.exists()) {
                    QDir().mkdir(dir.absolutePath());
                }

                file = new QFile(dir.absolutePath() + "/" + fileName, this);

                // Handle file conficts (already exists)
                if (file->exists()) {
                    QStringList filePath = file->fileName().split('.');
                    if (filePath.size() == 1) {
                        // File without suffix. For example, ./READEME
                        QString fileNameTemp = filePath.at(0);
                        for (int i = 2; i <= INT_MAX; i++) {
                            if (QFile::exists(fileNameTemp + " (" + QString::number(i) + ")"))
                                continue;
                            // Override the existing
                            file = new QFile(fileNameTemp + " (" + QString::number(i) + ")");
                            break;
                        }
                    } else {
                        // File with suffix. For example, ./README.md
                        QString fileNameBase = filePath[filePath.size() - 2];
                        for (int i = 2; i <= INT_MAX; i++) {
                            filePath[filePath.size() - 2] = fileNameBase + " (" + QString::number(i) + ")";
                            QString compiledPath = filePath.join('.');
                            if (QFile::exists(compiledPath))
                                continue;
                            // Override the existing
                            file = new QFile(compiledPath);
                            break;
                        }
                    }
                }
                if (file->open(QIODevice::WriteOnly)) {
                    qDebug() << "Sender: File opened successfully.";
                    status = ReceiverStatus::Transferring;
                    metaProcessed = true;
                } else {
                    qDebug() << "Sender: Failed to write ";
                    cancel();
                }
                break;
            }
            case PacketType::Complete: {
                emit statusUpdate(10000);
                qDebug() << "[Receiver] Complete!";
                file->close();
                socket->close();
                break;
            }
            case PacketType::Pause: {
                qDebug() << "Type: pause";
                break;
            }
            case PacketType::Resume: {
                qDebug() << "Type: resume";
                break;
            }
            case PacketType::Cancel: {
                qDebug() << "Type: cancel";
                return cancel();
            }
            default: {
                return error();
                qDebug() << "Receiver: Default case: " << socketBuffer;
                break;
            }
        }

        if (payloadSize == 0) {
            socketBuffer.remove(0, sizeof(type));
        } else {
            socketBuffer.remove(0, sizeof(type) + sizeof(qint32) + payloadSize);
        }

        if (socketBuffer.size() < payloadSize && status == ReceiverStatus::Transferring) {
            sendPacket(PacketType::ACK);
        }
    }
}

void FileReceiver::writeData(QByteArray& data)
{
    file->write(data);
    sizeProcessed += data.size();
    emit statusUpdate((int)((double)sizeProcessed * 10000 / fileSize));
}

void FileReceiver::pause()
{
    sendPacket(PacketType::Pause);
    status = ReceiverStatus::Paused;
}

void FileReceiver::resume()
{
    sendPacket(PacketType::Resume);
    status = ReceiverStatus::Transferring;
}

void FileReceiver::error()
{
    qDebug() << "[Receiver] Error detected";
    sendPacket(PacketType::Error);
    status = ReceiverStatus::Error;
    emit statusUpdate(0);
    if (file)
        file->close();
    file->remove();
}

void FileReceiver::cancel()
{
    qDebug() << "[Receiver] Cancel.";
    sendPacket(PacketType::Cancel);
    status = ReceiverStatus::Canceled;
    file->close();
    file->remove();
    socket->close();
    emit statusUpdate(0);
}
