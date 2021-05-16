#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QTcpServer>
#include <QString>
#include <QSsl>
#include <QSslCertificate>
#include <QSslKey>
#include <QSslSocket>
#include <QFile>

#include "sslsocket.h"

class SslServer : public QTcpServer
{
    Q_OBJECT

public:
    SslServer(QObject *parent = 0);

    const QSslCertificate &getSslLocalCertificate() const;
    const QSslKey &getSslPrivateKey() const;
    QSsl::SslProtocol getSslProtocol() const;

    void setSslLocalCertificate(const QSslCertificate &certificate);
    bool setSslLocalCertificate(const QString &path, QSsl::EncodingFormat format = QSsl::Pem);

    void setSslPrivateKey(const QSslKey &key);
    bool setSslPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm = QSsl::Rsa, QSsl::EncodingFormat format = QSsl::Pem, const QByteArray &passPhrase = QByteArray());

    void setSslProtocol(QSsl::SslProtocol protocol);

protected:
    void incomingConnection(qintptr socketDescriptor) override final;

private:
    QSslCertificate userSslLocalCertificate;
    QSslKey userSslPrivateKey;
    QSsl::SslProtocol userSslProtocol;

private slots:
    void onSslErrors(const QList<QSslError>&);
};

#endif // SSLSERVER_H
