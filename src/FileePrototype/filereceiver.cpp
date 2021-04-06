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

}

void FileReceiver::readPacket()
{
    socketBuffer.append(socket->readAll());

    while (socketBuffer.size() > 0) {
        PacketType type = static_cast<PacketType>(socketBuffer.at(0));
        qint32 payloadSize = 0;

        switch (type) {
            case PacketType::Data: {
                if (status != ReceiverStatus::Transferring)
                    return;
                memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
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
                qDebug() << "Type: meta";
                payloadSize = socketBuffer.at(1);
                QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
                QJsonObject obj = QJsonDocument::fromJson(data).object();
                fileSize = obj.value("size").toVariant().value<qint64>();
                fileName = obj.value("name").toString();

                QProcessEnvironment env(QProcessEnvironment::systemEnvironment());

                QDir dir(env.value("USERPROFILE") + "\\Downloads\\Filee");
                if (!dir.exists()) {
                    QDir().mkdir(env.value("USERPROFILE") + "\\Downloads\\Filee");
                }
                qDebug() << dir;

                file = new QFile(env.value("USERPROFILE") + "\\Downloads\\Filee\\" + fileName, this);
                if (file->open(QIODevice::WriteOnly)) {
                    qDebug() << "Sender: File opened successfully.";
                    status = ReceiverStatus::Transferring;
                } else {
                    qDebug() << "Sender: Failed to write ";
                }
                break;
            }
            case PacketType::Complete: {
                emit statusUpdate(100);
                qDebug() << "[Receiver] Complete!!";
                file->close();
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
                break;
            }
            default: {
                qDebug() << "Receiver: Default case";
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

void FileReceiver::writeData(QByteArray& data)
{
    file->write(data);
    sizeProcessed += data.size();
    emit statusUpdate((int)((double)sizeProcessed * 100 / fileSize));
}
