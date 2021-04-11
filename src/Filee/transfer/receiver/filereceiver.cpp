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
                if (payloadSize + (qint64)(sizeof(type) + sizeof(qint32)) > socketBuffer.size()) {
//                    sendPacket(PacketType::RequestData);
                    return;
                }
                if (status != ReceiverStatus::Syncing) {
                    QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                    writeData(data);
                }
                break;
            }
            case PacketType::SendRequest: {
                qDebug() << "Type: request" << socketBuffer;
                if (metaProcessed)
                    return error();
                memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                QJsonObject obj = QJsonDocument::fromJson(data).object();
                fileSize = obj.value("size").toVariant().value<qint64>();
                fileName = obj.value("name").toString();

                qDebug() << "File name: " << fileName;
                double fileSizeDisplay = fileSize;
                int unitIdx = 0;
                while (fileSizeDisplay > 1024) {
                    if (unitIdx > sizeUnits.size())
                        break;
                    fileSizeDisplay /= 1024;
                    unitIdx++;
                }
                QString fileSizeUnit = sizeUnits.at(unitIdx);

                QMessageBox msgBox;
                msgBox.setText(socket->peerAddress().toString() + " wants to transfer a file to you.");
                msgBox.setInformativeText("File name: " + fileName + "\n"
                                          + "File size: " + QString::number(fileSizeDisplay, 'f', 2) + " " + fileSizeUnit + "\n"
                                          + "Do you want to accept the file?");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Ok);
                int ret = msgBox.exec();

                if (ret != QMessageBox::Ok) {
                    sendPacket(PacketType::Deny);
                    socket->close();
                    return;
                }

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
                    sendPacket(PacketType::Accept);
                    sendPacket(PacketType::RequestData);
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
            case PacketType::ConfirmSync: {
                qDebug() << "Type: ConfirmSync";
                if (status != ReceiverStatus::Syncing) {
                    qDebug() << "Syncing packet received while not syncing, could be serious congestion, aborting";
                    return error();
                }
                try {
                    memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
                } catch (...) {
                    return;
                }

                if (payloadSize + (qint64)(sizeof(type) + sizeof(qint32)) > socketBuffer.size()) {
//                    sendPacket(PacketType::RequestData);
                    return;
                }
                // The packet now contains valid data
                qint64 pos = 0;
                memcpy(&pos, socketBuffer.mid(sizeof(type) + sizeof(qint32), sizeof(qint64)), sizeof(qint64));
                qDebug() << "[Receiver] Received SYNC to position " << pos;
                if (pos != sizeProcessed) {
                    qDebug() << "Wrong pos rewinded, aborting...";
                    return error();
                }
                sendPacket(PacketType::SyncDone);
                status = ReceiverStatus::Transferring;
                sendPacket(PacketType::RequestData);
                break;
            }
            default: {
                return overloaded();
//                return error();
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
            sendPacket(PacketType::RequestData);
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

void FileReceiver::overloaded()
{
    qDebug() << "[Receiver] Overloaded! Need to rewind to " << sizeProcessed;
    if (status == ReceiverStatus::Syncing)
        return;
    status = ReceiverStatus::Syncing;
    QByteArray packet(reinterpret_cast<const char *>(&sizeProcessed), sizeof(sizeProcessed));
    sendPacket(PacketType::SyncRequest, packet);
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
