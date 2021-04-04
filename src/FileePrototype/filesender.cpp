#include "filesender.h"

FileSender::FileSender(QString filePath, QHostAddress receiverAddress, QObject *parent)
    : QObject(parent), fileDir(filePath)
{
    fileBufferSize = BUFFER_SIZE;
    fileBuffer.resize(fileBufferSize);

    file = new QFile(fileDir, this);
    bool opened = file->open(QIODevice::ReadOnly);

    if (!opened)
        return;

    if (file->size() == 0)
        return;

    fileSize = file->size();
    remainingSize = fileSize;

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(receiverAddress, 3800, QAbstractSocket::ReadWrite);

    connect(tcpSocket, &QTcpSocket::bytesWritten, this, &FileSender::socketBytesWritten);
    connect(tcpSocket, &QTcpSocket::connected, this, &FileSender::socketConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &FileSender::socketDisconnected);

    qDebug() << "Constrct()";

    // Send meta info
    sendMeta();
}

void FileSender::socketBytesWritten()
{
//    qDebug() << "Write";
    sendData();
}

void FileSender::socketConnected()
{
//    qDebug() << "Connected";
    sendData();
}

void FileSender::socketDisconnected()
{
//    qDebug() << "Disconnected.";
}

void FileSender::sendMeta() {
    QString fileName = QDir(file->fileName()).dirName();
    QJsonObject obj(QJsonObject::
                    fromVariantMap({{"name", fileName},{"size", fileSize},}));
    QByteArray packet(QJsonDocument(obj).toJson());
    tcpSocket->write(packet, 255);
}

void FileSender::sendData()
{
    if (remainingSize < fileBufferSize) {
        fileBufferSize = remainingSize;
        fileBuffer.resize(remainingSize);
    }
    qDebug() << "Reading...";
    qint64 bytesRead = file->read(fileBuffer.data(), fileBufferSize);
    if (bytesRead == -1)
        return;

    remainingSize -= bytesRead;
    if (remainingSize < 0)
        remainingSize = 0;

    // Emit progress change
//    qDebug() << (int) ((fileSize-remainingSize)/ fileSize * 100);
    tcpSocket->write(fileBuffer);

    remainingSize -= bytesRead;
    if (remainingSize <= 0) {
        remainingSize = 0;
        file->close();
        tcpSocket->close();
    }
}


