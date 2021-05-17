#include "filereceiver.h"

FileReceiver::FileReceiver(QTcpSocket *tcpSocket, QObject *parent)
    : FileTransferPeer(parent)
{
    setSocket(tcpSocket);

    connectSlots();
}

FileReceiver::FileReceiver(QObject *parent)
    : FileTransferPeer(parent)
{

}

void FileReceiver::connectSlots()
{
    connect(getSocket(), &QTcpSocket::disconnected, this, &FileReceiver::socketConnected);
    connect(getSocket(), &QTcpSocket::readyRead, this, &FileReceiver::readPacket);
}

void FileReceiver::socketConnected()
{

}

void FileReceiver::socketDisconnected()
{
    closeSocket();
}

void FileReceiver::readPacket()
{
    sizeTransferred += getBytesAvailable();
    socketBuffer.append(readSocketBuffer());

    while (socketBuffer.size() > 0) {
        PacketType type = static_cast<PacketType>(socketBuffer.at(0));
        qint64 payloadSize = 0;

        switch (type) {
            case PacketType::Data: {
                if (status == ReceiverStatus::Error)
                    return;
                if (status != ReceiverStatus::Transferring)
                    return;
                try {
                    memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                } catch (...) {
                    return;
                }
                if (payloadSize + (qint64)(sizeof(type) + sizeof(qint32)) > socketBuffer.size())
                    return;
                if (status != ReceiverStatus::Syncing) {
                    QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                    writeData(data);
                }
                break;
            }
            case PacketType::SendRequest: {
                if (metaProcessed)
                    return error();
                memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                QJsonObject obj = QJsonDocument::fromJson(data).object();
                fileSize = obj.value("size").toVariant().value<qint64>();
                filename = obj.value("name").toString();

                QMessageBox msgBox;
                msgBox.setText(getPeerAddress().toString() + " wants to transfer a file to you.");
                msgBox.setInformativeText("File name: " + filename + "\n"
                                          + "File size: " + Common::humanReadableSize(fileSize) + "\n"
                                          + "Do you want to accept the file?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();

                if (ret != QMessageBox::Ok) {
                    sendPacket(PacketType::Deny);
                    closeSocket();
                    return;
                }

                QDir dir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/Filee");
                if (!dir.exists()) {
                    QDir().mkdir(dir.absolutePath());
                }

                file = new QFile(dir.absolutePath() + "/" + filename, this);

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
                    status = ReceiverStatus::Transferring;
                    metaProcessed = true;
                    sendPacket(PacketType::Accept);
                    sendPacket(PacketType::RequestData);
                    emit receiverBegin(getPeerAddress().toString(),
                                       filename, file->fileName());
                } else {
                    MessageBox::messageBoxCritical("Unable to open to write.");
                    cancel();
                }
                startRateMeter();
                break;
            }
            case PacketType::Complete: {
                emit receiverStatusUpdate(10000);
                file->close();
                closeSocket();
                emit receiverEnded();
                stopRateMeter();
                break;
            }
            case PacketType::Pause: {
                break;
            }
            case PacketType::Resume: {
                break;
            }
            case PacketType::Cancel: {
                emit receiverTerminated();
                MessageBox::messageBoxCritical("The transfer was terminated by the remote peer.");
                return cancel();
            }
            case PacketType::ConfirmSync: {
                if (status != ReceiverStatus::Syncing) {
                    // Syncing packet received while not syncing, could be serious congestion, aborting
                    return error();
                }
                try {
                    memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                } catch (...) {
                    return;
                }

                if (payloadSize + (qint64)(sizeof(type) + sizeof(qint32)) > socketBuffer.size())
                    return;

                // The packet now contains valid data
                qint64 pos = 0;
                memcpy(&pos, socketBuffer.mid(sizeof(type) + sizeof(qint32), sizeof(qint64)), sizeof(qint64));
                if (pos != sizeProcessed) {
                    return error();
                }
                sendPacket(PacketType::SyncDone);
                status = ReceiverStatus::Transferring;
                sendPacket(PacketType::RequestData);
                break;
            }
            default: {
                return overloaded();
                break;
            }
        }

        if (payloadSize == 0) {
            socketBuffer.remove(0, sizeof(type));
        } else {
            socketBuffer.remove(0, sizeof(type) + sizeof(qint32) + payloadSize);
        }

        if (socketBuffer.size() < payloadSize && status == ReceiverStatus::Transferring) {
            sendPacket(PacketType::RequestData);
        }
    }
}

void FileReceiver::writeData(QByteArray& data)
{
    file->write(data);
    sizeProcessed += data.size();
    emit receiverStatusUpdate((int)((double)sizeProcessed * 10000 / fileSize));
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
    sendPacket(PacketType::Error);
    status = ReceiverStatus::Error;
    emit receiverStatusUpdate(0);
    if (file)
        file->close();
    // file->remove();
    stopRateMeter();
}

void FileReceiver::overloaded()
{
    if (status == ReceiverStatus::Syncing)
        return;
    status = ReceiverStatus::Syncing;
    QByteArray packet(reinterpret_cast<const char *>(&sizeProcessed), sizeof(sizeProcessed));
    sendPacket(PacketType::SyncRequest, packet);
    stopRateMeter();
}

void FileReceiver::cancel()
{
    sendPacket(PacketType::Cancel);
    status = ReceiverStatus::Canceled;
    file->close();
    file->remove();
    closeSocket();
    emit receiverStatusUpdate(0);
    stopRateMeter();
}
