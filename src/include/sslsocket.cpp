#include "sslsocket.h"

SslSocket::SslSocket(QObject *parent)
    : QSslSocket(parent)
{
    // The expected errors from self-signed certificates
    expectedErrors.push_back(QSslError::SelfSignedCertificate);
    expectedErrors.push_back(QSslError::HostNameMismatch);

    // ignoreSslErrors(expectedErrors);

    // Test bugs only
    QFile testFile("a.txt");
    qDebug() << testFile.open(QIODevice::ReadOnly);
    qDebug() << testFile.readAll();

    QFile certificateFile("a.txt");
    qDebug() << certificateFile.readAll();
    if (!certificateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Certificate file not found.";
        return;
    }
    QSslCertificate sslCertificate(certificateFile.readAll(), QSsl::Pem);
    this->setLocalCertificate(sslCertificate);

    QFile keyFile("b.txt");
    qDebug() << "File location:" << QFileInfo("private.pem").absoluteFilePath();
    if (!keyFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Private key file not found.";
        return;
    }
    QSslKey sslKey(keyFile.readAll(), QSsl::Rsa);
    this->setPrivateKey(QSslKey(keyFile.readAll(), QSsl::Rsa));

    qDebug() << keyFile.readAll();
    qDebug() << certificateFile.readAll();
    qDebug() << "Is certificate null? " << sslCertificate.isNull();
    qDebug() << "Is private key null? " << sslKey.isNull();

    QList<QSslCertificate> trustedCas;
    trustedCas.push_back(sslCertificate);
    if (trustedCas.empty()) {
        qDebug() << "Error: no trusted Cas";
        return;
    }
    this->setCaCertificates(trustedCas);

    this->setProtocol(QSsl::TlsV1_2);
    // this->setPeerVerifyMode(QSslSocket::VerifyPeer);

    connect(this, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
        [=](const QList<QSslError> &errors) {
        for (auto error : errors) {
            qDebug() << "[SSL Error]" << error;
        }
    });

    qDebug() << "SSL Socket initialized";

    keyFile.close();
    certificateFile.close();
}
