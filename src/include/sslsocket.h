#ifndef SSLSOCKET_H
#define SSLSOCKET_H

#include <QSslSocket>
#include <QObject>

class SslSocket : public QSslSocket
{
    Q_OBJECT
public:
    explicit SslSocket(QObject* parent = 0);
//    virtual ~SslSocket();

protected:
    QList<QSslError> expectedErrors;

//protected slots:
//    virtual void onSslErrors(const QList<QSslError>&);

};

#endif // SSLSOCKET_H
