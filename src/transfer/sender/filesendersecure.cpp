#include "filesendersecure.h"
#include "../filetransferpeersecure.h"

FileSenderSecure::FileSenderSecure(QString filePath, QHostAddress receiverAddress, qint64 bufferSize, QObject *parent)
    : FileSender(parent)
{
    fileBufferSize = bufferSize;
    fileDir = filePath;
    fileBuffer.resize(fileBufferSize);

    file = new QFile(fileDir, this);
    bool opened = file->open(QIODevice::ReadOnly);

    if (!opened) {
        MessageBox::messageBoxCritical("Unable to open the file to read.");
        return;
    }

    if (file->size() == 0)
        return;

    fileSize = file->size();
    sizeProcessed = 0;

    socket = new QSslSocket(this);
    socket->setProxy(QNetworkProxy::NoProxy);
    socket->connectToHostEncrypted(receiverAddress.toString(), 3801);

    FileTransferPeer::setSocket(socket);

    connectSlots();
}
