#include "filereceiver.h"

FileReceiver::FileReceiver(QTcpSocket *tcpSocket, QObject *parent)
    : FileTransferPeer(parent)
{
    socket = tcpSocket;

    connect(socket, &QTcpSocket::disconnected, this, &FileReceiver::socketConnected);
    connect(socket, &QTcpSocket::readyRead, this, &FileReceiver::readPacket);

    file = new QFile("C:\\Downloads\\test.txt", this);
    if (file->open(QIODevice::WriteOnly)) {
        qDebug() << "Sender: File opened successfully.";
    } else {
        qDebug() << "Sender: Failed to write ";
    }
}

void FileReceiver::metaParser(QByteArray& data)
{
//    int stop = 0;
//    while(data.at(stop) != '\0') {
//        stop++;
//    }
//    QJsonObject obj = QJsonDocument::fromJson(data.mid(0, stop-1)).object();
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    fileSize = obj.value("size").toVariant().value<qint64>();
    fileName = obj.value("name").toString();
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
//                qDebug() << "Type: data";
                memcpy(&payloadSize, socketBuffer.mid(sizeof(type), sizeof(qint32)), sizeof(qint32));
//                qDebug() << "Payload size test: " << socketBuffer.mid(sizeof(type), sizeof(qint32));
//                payloadSize = static_cast<qint32>(socketBuffer.at(1));
//                qDebug() << "Payload size: " << payloadSize;
                QByteArray data = socketBuffer.mid(sizeof(type) + sizeof(qint32), payloadSize);
//                qDebug() << "Data: " << data;
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
                metaParser(data);
                break;
            }
            case PacketType::Complete: {
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
}
