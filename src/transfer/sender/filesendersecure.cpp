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

    // Setup the SslSocket
    socket = new SslSocket(this);
    socket->setProxy(QNetworkProxy::NoProxy);
    // The default behavior clients
    socket->setPeerVerifyMode(QSslSocket::VerifyPeer);

    // Trust the server certificate
    QList<QSslCertificate> trustedCas;
    QFile serverCertFile("servercert.pem");
    if (!serverCertFile.open(QIODevice::ReadOnly)) {
        MessageBox::messageBoxCritical("You need the receiver's certificate installed before the transmission "
            "to ensure the security since the certificate is self-signed. TLS connection aborted.");
        return;
    }
    QSslCertificate serverCert(serverCertFile.readAll(), QSsl::Pem);
    trustedCas.push_back(serverCert);
    socket->setCaCertificates(trustedCas);
    serverCertFile.close();

    socket->connectToHostEncrypted(receiverAddress.toString(), 3801);
    if (!socket->waitForEncrypted()) {
        MessageBox::messageBoxCritical("Unable to establish secure connection with the receiver. "
            "Please make sure the receiver has correctly setup TLS.");
        return;
    }
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
