#include "filesendersecure.h"

FileSenderSecure::FileSenderSecure(QString filePath, QHostAddress receiverAddress, qint64 bufferSize, QObject *parent)
    : FileSender(parent)
{
    fileBufferSize = bufferSize;
    fileDir = filePath;
    fileBuffer.resize(fileBufferSize);

    file = new QFile(fileDir, this);
    bool opened = file->open(QIODevice::ReadOnly);

    if (!opened) {
        MessageBox::messageBoxCritical("Unable to open the file to read.");
        return;
    }

    if (file->size() == 0)
        return;

    fileSize = file->size();
    sizeProcessed = 0;

    socket = new SslSocket(this);
    // socket->setProxy(QNetworkProxy::NoProxy);
    qDebug() << "p1";
    socket->connectToHostEncrypted(receiverAddress.toString(), 3801);
    qDebug() << "p2";
//    if (!socket->waitForEncrypted()) {
//        qDebug() << socket->errorString();
//        return;
//    }
    qDebug() << "p3";

    setSecureSocket(socket);

    connectSlots();
}

FileSenderSecure::~FileSenderSecure()
{
    disconnect(getSecureSocket(), &SslSocket::bytesWritten, this, &FileSenderSecure::socketBytesWritten);
    disconnect(getSecureSocket(), &SslSocket::connected, this, &FileSenderSecure::socketConnected);
    disconnect(getSecureSocket(), &SslSocket::disconnected, this, &FileSenderSecure::socketDisconnected);
    disconnect(getSecureSocket(), &SslSocket::readyRead, this, &FileSenderSecure::readPacket);
}

void FileSenderSecure::connectSlots()
{
    connect(getSecureSocket(), &SslSocket::bytesWritten, this, &FileSenderSecure::socketBytesWritten);
    connect(getSecureSocket(), &SslSocket::connected, this, &FileSenderSecure::socketConnected);
    connect(getSecureSocket(), &SslSocket::disconnected, this, &FileSenderSecure::socketDisconnected);
    connect(getSecureSocket(), &SslSocket::readyRead, this, &FileSenderSecure::readPacket);
}
