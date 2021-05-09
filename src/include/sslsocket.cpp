#include "sslsocket.h"

SslSocket::SslSocket(QObject *parent)
    : QSslSocket(parent)
{
    // The expected errors from self-signed certificates
    expectedErrors.push_back(QSslError::SelfSignedCertificate);
    expectedErrors.push_back(QSslError::HostNameMismatch);

    ignoreSslErrors(expectedErrors);
}
