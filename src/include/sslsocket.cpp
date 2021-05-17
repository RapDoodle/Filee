#include "sslsocket.h"

SslSocket::SslSocket(QObject *parent)
    : QSslSocket(parent)
{
    // The expected errors from self-signed certificates
    expectedErrors.push_back(QSslError::SelfSignedCertificate);
    expectedErrors.push_back(QSslError::HostNameMismatch);

    ignoreSslErrors(expectedErrors);

    QFile certificateFile("cert.pem");
    if (!certificateFile.open(QIODevice::ReadOnly)) {
        MessageBox::messageBoxCritical("Unable to open the certificate. TLS connection aborted.");
        return;
    }
    QSslCertificate sslCertificate(certificateFile.readAll(), QSsl::Pem);
    this->setLocalCertificate(sslCertificate);

    QFile keyFile("private.pem");
    if (!keyFile.open(QIODevice::ReadOnly)) {
        MessageBox::messageBoxCritical("Unable to open the private key. TLS connection aborted.");
        return;
    }
    QSslKey sslKey(keyFile.readAll(), QSsl::Rsa);
    this->setPrivateKey(QSslKey(keyFile.readAll(), QSsl::Rsa));

    if (sslCertificate.isNull() || sslKey.isNull()) {
        MessageBox::messageBoxCritical("Your private key or certificate is corrupted. TLS connection aborted.");
    }

    this->setProtocol(QSsl::TlsV1_2);

    // The default behavior for clients
    this->setPeerVerifyMode(QSslSocket::VerifyPeer);

    connect(this, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
        [=](const QList<QSslError> &errors) {
        for (auto error : errors) {
            qDebug() << "[SSL Error]" << error;
        }
    });

    keyFile.close();
    certificateFile.close();
}
