#include "transfersession.h"

TransferSession::TransferSession(QString filePath, QHostAddress receiverAddress, QObject *parent) :
    QObject(parent), fileDir(filePath), address(receiverAddress)
{
    bufferSize = 1024 * 8192;

    #if defined (Q_OS_ANDROID)
    bufferSize = 1024 * 2048;
    #endif

    transfer();
}

void TransferSession::transfer()
{
    sender = new FileSender(fileDir, address, bufferSize);
    connect(sender, &FileSender::transferAborted, this, &TransferSession::overloadedHandler);
    connect(sender, QOverload<int>::of(&FileSender::statusUpdate),
            this, QOverload<int>::of(&TransferSession::senderProgressChanged));
}

void TransferSession::deleteConnection()
{
    disconnect(sender, QOverload<int>::of(&FileSender::statusUpdate),
               this, QOverload<int>::of(&TransferSession::senderProgressChanged));
    disconnect(sender, &FileSender::transferAborted, this, &TransferSession::overloadedHandler);
}

void TransferSession::overloadedHandler()
{
    deleteConnection();
//    delete sender;
//    sender = nullptr;
    bufferSize = bufferSize / 2 > 16 ? bufferSize / 2 : 16;
    attempt++;
    if (attempt > 16)
        return endSession();

    transfer();
}

void TransferSession::endSession()
{
    deleteConnection();
    delete sender;
    emit sessionAborted();
}

void TransferSession::pause()
{
    if (sender)
        sender->pause();
}

void TransferSession::resume()
{
    if (sender)
        sender->resume();
}

void TransferSession::cancel()
{
    if (sender)
        sender->cancel();
}

void TransferSession::senderProgressChanged(int progress)
{
    emit progressUpdate(progress);
}

/* Slots */
void TransferSession::completed()
{
    emit sessionCompleted();
}

void TransferSession::canceled()
{
    delete sender;
    emit sessionAborted();
}

