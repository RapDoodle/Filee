#include "filereceiver.h"

FileReceiver::FileReceiver(QTcpSocket *socket, QObject *parent)
    : QObject(parent), tcpSocket(socket)
{
    connect(socket, &QTcpSocket::disconnected, this, &FileReceiver::socketConnected);
    connect(socket, &QTcpSocket::readyRead, this, &FileReceiver::readPacket);
}

void FileReceiver::metaParser(QByteArray& data)
{
    int stop = 0;
    while(data.at(stop) != '\0') {
        stop++;
    }
    QJsonObject obj = QJsonDocument::fromJson(data.mid(0, stop-1)).object();
    fileSize = obj.value("size").toVariant().value<qint64>();
    fileName = obj.value("name").toString();

    file = new QFile("C:\\Downloads\\test.txt", this);
    if (file->open(QIODevice::WriteOnly)) {
        qDebug() << "Opened successfully.";
    } else {
        qDebug() << "Failed to write ";
    }
}

void FileReceiver::socketConnected()
{

}

void FileReceiver::socketDisconnected()
{

}

void FileReceiver::readPacket()
{
    fileBuffer.append(tcpSocket->readAll());

    while (fileBuffer.size() > 0) {
        if (packetCount == 0) {
            QByteArray data = fileBuffer.mid(0, 255);
            metaParser(data);
            fileBuffer.remove(0, 255);
        } else {
            writeData(fileBuffer);
            fileBuffer.clear();
        }
        qDebug() << packetCount;
        packetCount++;
    }
}

void FileReceiver::writeData(QByteArray& data)
{
    file->write(data);
    downloadedSize += data.size();
    file->flush();
    if (downloadedSize >= fileSize)
        file->close();
    // If it is the last write, close the file
}
