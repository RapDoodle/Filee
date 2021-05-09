#ifndef FILESENDERSECURE_H
#define FILESENDERSECURE_H

#include "filesender.h"
#include <QObject>
#include <QSslSocket>

class FileSenderSecure : public FileSender
{
    Q_OBJECT
public:
    explicit FileSenderSecure(QString filePath, QHostAddress receiverAddress, qint64 bufferSize, QObject *parent = nullptr);

protected:
    QSslSocket* socket = nullptr;
};

#endif // FILESENDERSECURE_H
