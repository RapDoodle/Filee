#ifndef FILERECEIVERSECURE_H
#define FILERECEIVERSECURE_H

#include "filereceiver.h"
#include <QObject>
#include <QSslSocket>

class FileReceiverSecure : public FileReceiver
{
    Q_OBJECT
public:
    explicit FileReceiverSecure(SslSocket *tcpSocket, QObject *parent = nullptr);

private:
    void connectSlots();

private slots:
    void onEncrypted();
};

#endif // FILERECEIVERSECURE_H
