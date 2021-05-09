#include "sslserver.h"

SslServer::SslServer(QObject *parent) : QTcpServer(parent),
    userSslLocalCertificate(),
    userSslPrivateKey(),
    userSslProtocol(QSsl::UnknownProtocol)
{

}

void SslServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setLocalCertificate(userSslLocalCertificate);
    sslSocket->setPrivateKey(userSslPrivateKey);
    sslSocket->setProtocol(userSslProtocol);
    sslSocket->startServerEncryption();
    sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);

    this->addPendingConnection(sslSocket);
}

bool SslServer::setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format)
{
    QFile certificateFile(path);

    if (!certificateFile.open(QIODevice::ReadOnly))
        return false;

    userSslLocalCertificate = QSslCertificate(certificateFile.readAll(), format);
    return true;
}

bool SslServer::setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format, const QByteArray &passPhrase)
{
    QFile keyFile(fileName);

    if (!keyFile.open(QIODevice::ReadOnly))
        return false;

    userSslPrivateKey = QSslKey(keyFile.readAll(), algorithm, format, QSsl::PrivateKey, passPhrase);
    return true;
}

void SslServer::setSslProtocol(QSsl::SslProtocol protocol) { userSslProtocol = protocol; }

void SslServer::setSslLocalCertificate(const QSslCertificate &certificate) { userSslLocalCertificate = certificate; }

void SslServer::setSslPrivateKey(const QSslKey &key) { userSslPrivateKey = key; }

const QSslCertificate &SslServer::getSslLocalCertificate() const { return userSslLocalCertificate; }

const QSslKey &SslServer::getSslPrivateKey() const { return userSslPrivateKey; }

QSsl::SslProtocol SslServer::getSslProtocol() const { return userSslProtocol; }
