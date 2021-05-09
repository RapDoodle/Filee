#include "filereceiversecure.h"

FileReceiverSecure::FileReceiverSecure(QSslSocket *sslSocket, QObject *parent)
    : FileReceiver(parent)
{
    socket = sslSocket;

    FileTransferPeer::setSocket(socket);

    connectSlots();
}
