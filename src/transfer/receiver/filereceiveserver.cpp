#include "filereceiveserver.h"
#include <QDir>

FileReceiveServer::FileReceiveServer(QObject *parent) : QObject(parent)
{
    // The server without TLS
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::AnyIPv4, 3800);
    connect(tcpServer, &QTcpServer::newConnection, this, &FileReceiveServer::newConnection);

    // Start a server with TLS
    // Check whether the build supports TLS
    if (QSslSocket::supportsSsl()) {
        if (QFile::exists("cert.pem") && QFile::exists("private.pem")) {
            sslServer = new SslServer(this);
            sslServer->setSslLocalCertificate("cert.pem");
            sslServer->setSslPrivateKey("private.pem");
            sslServer->setSslProtocol(QSsl::TlsV1_2);
            sslServer->listen(QHostAddress::AnyIPv4, 3801);
            connect(sslServer, &SslServer::newConnection, this, &FileReceiveServer::newSecureConnection);
        } else {
            MessageBox::messageBoxWarning("Certificate for TLS not found. TLS service is not running.");
        }
    } else {
        MessageBox::messageBoxWarning("TLS is not supported in the current build. "
            "Please check your build configurations. For more information, please refer "
            "to the README.md included.");
    }
}

void FileReceiveServer::newConnection()
{
    QTcpSocket* socket = tcpServer->nextPendingConnection();
    if (socket) {
        FileReceiver* receiver = new FileReceiver(socket);
        receivers.push_back(receiver);
        connect(receiver, &FileReceiver::receiverBegin, this, [&](QString sender, QString filename, QString filePath) {
            emit receiverBegin(sender, filename, filePath);
        });
        connect(receiver, &FileReceiver::receiverEnded, this, [&]() {
            emit receiverEnded();
        });
        connect(receiver, &FileReceiver::receiverTerminated, this, [&]() {
            emit receiverTerminated();
        });
        connect(receiver, QOverload<int>::of(&FileReceiver::receiverStatusUpdate),
                this, [&](int status) {
            emit receiverStatusUpdate(status);
        });
        connect(receiver, QOverload<QString>::of(&FileReceiver::rateUpdate),
                this, [&](QString rate) {
            emit rateUpdate(rate);
        });
    }
}

void FileReceiveServer::newSecureConnection()
{
    SslSocket* socket = (SslSocket*) sslServer->nextPendingConnection();
    socket->ignoreSslErrors();

    if (socket) {
        FileReceiverSecure* receiver = new FileReceiverSecure(socket);
        receivers.push_back(receiver);
        connect(receiver, &FileReceiverSecure::receiverBegin, this, [&](QString sender, QString filename, QString filePath) {
            emit receiverBegin(sender, filename, filePath);
        });
        connect(receiver, &FileReceiverSecure::receiverEnded, this, [&]() {
            emit receiverEnded();
        });
        connect(receiver, &FileReceiverSecure::receiverTerminated, this, [&]() {
            emit receiverTerminated();
        });
        connect(receiver, QOverload<int>::of(&FileReceiverSecure::receiverStatusUpdate),
                this, [&](int status) {
            emit receiverStatusUpdate(status);
        });
        connect(receiver, QOverload<QString>::of(&FileReceiverSecure::rateUpdate),
                this, [&](QString rate) {
            emit rateUpdate(rate);
        });
    }
}

void FileReceiveServer::pause()
{
    for (auto socket : receivers) {
        socket->pause();
    }
}

void FileReceiveServer::resume()
{
    for (auto socket : receivers) {
        socket->resume();
    }
}

void FileReceiveServer::cancel()
{
    for (auto socket : receivers) {
        socket->cancel();
    }
}
