#ifndef FILERECEIVERSECURE_H
#define FILERECEIVERSECURE_H

#include "filereceiver.h"
#include <QObject>
#include <QSslSocket>

class FileReceiverSecure : public FileReceiver
{
    Q_OBJECT
public:
    explicit FileReceiverSecure(QSslSocket *tcpSocket, QObject *parent = nullptr);

protected:
    QSslSocket* socket = nullptr;
};

#endif // FILERECEIVERSECURE_H
