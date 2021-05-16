#ifndef FILESENDERSECURE_H
#define FILESENDERSECURE_H

#include "filesender.h"
#include <QObject>
#include <QSslSocket>
#include "../../include/sslsocket.h"

class FileSenderSecure : public FileSender
{
    Q_OBJECT
public:
    explicit FileSenderSecure(QString filePath, QHostAddress receiverAddress, qint64 bufferSize, QObject *parent = nullptr);
    ~FileSenderSecure();

protected:
    SslSocket* socket = nullptr;

private:
    void connectSlots();
};

#endif // FILESENDERSECURE_H
