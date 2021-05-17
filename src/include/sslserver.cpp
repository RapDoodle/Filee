#include "sslserver.h"

SslServer::SslServer(QObject *parent) : QTcpServer(parent),
    userSslLocalCertificate(),
    userSslPrivateKey(),
    userSslProtocol(QSsl::UnknownProtocol)
{

}

void SslServer::incomingConnection(qintptr socketDescriptor)
{
    SslSocket *sslSocket = new SslSocket(this);

    QList<QSslError> expectedErrors;
    expectedErrors.push_back(QSslError::SelfSignedCertificate);
    expectedErrors.push_back(QSslError::HostNameMismatch);
    sslSocket->ignoreSslErrors(expectedErrors);
    // The default behavior for servers
    sslSocket->setPeerVerifyMode(QSslSocket::QueryPeer);

    if (sslSocket->setSocketDescriptor(socketDescriptor, QSslSocket::ConnectedState, QSslSocket::ReadWrite)) {
        sslSocket->setLocalCertificate(userSslLocalCertificate);
        sslSocket->setPrivateKey(userSslPrivateKey);
        sslSocket->setProtocol(userSslProtocol);
        sslSocket->setPeerVerifyMode(QSslSocket::QueryPeer);
        // sslSocket->setPeerVerifyMode(QSslSocket::VerifyPeer);
        sslSocket->startServerEncryption();
        this->addPendingConnection(sslSocket);
    } else {
        MessageBox::messageBoxCritical("Failed to set file descriptor for TLS socket. Please check your"
            " OpenSSL build supports TLSv1.2.");
    }

}

bool SslServer::setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format)
{
    QFile certificateFile(path);

    if (!certificateFile.open(QIODevice::ReadOnly))
        return false;

    userSslLocalCertificate = QSslCertificate(certificateFile.readAll(), format);
    certificateFile.close();
    return true;
}

bool SslServer::setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format, const QByteArray &passPhrase)
{
    QFile keyFile(fileName);

    if (!keyFile.open(QIODevice::ReadOnly))
        return false;

    userSslPrivateKey = QSslKey(keyFile.readAll(), algorithm, format, QSsl::PrivateKey, passPhrase);
    keyFile.close();
    return true;
}

void SslServer::onSslErrors(const QList<QSslError> &errors) {
    foreach (QSslError err, errors) {

        fprintf(stderr, "QSslSocket error (# %d): %s\n",
                (int) err.error(),
                qPrintable(err.errorString())
                );
        qDebug() << "Warning: ";
        qDebug() << err.errorString();
    }
}

void SslServer::setSslProtocol(QSsl::SslProtocol protocol) { userSslProtocol = protocol; }

void SslServer::setSslLocalCertificate(const QSslCertificate &certificate) { userSslLocalCertificate = certificate; }

void SslServer::setSslPrivateKey(const QSslKey &key) { userSslPrivateKey = key; }

const QSslCertificate &SslServer::getSslLocalCertificate() const { return userSslLocalCertificate; }

const QSslKey &SslServer::getSslPrivateKey() const { return userSslPrivateKey; }

QSsl::SslProtocol SslServer::getSslProtocol() const { return userSslProtocol; }
