#include "transfersession.h"

TransferSession::TransferSession(QString filePath, QHostAddress receiverAddress, QObject *parent) :
    QObject(parent), fileDir(filePath), address(receiverAddress)
{
    bufferSize = 1024 * 8192;

    #if defined (Q_OS_ANDROID)
    bufferSize = 1024 * 4096;
    #endif

    transfer();
}

void TransferSession::transfer()
{
    sender = new FileSender(fileDir, address, bufferSize);
    connect(sender, &FileSender::restartRequest, this, &TransferSession::overloadedHandler);
    connect(sender, &FileSender::senderBegin, this, [&]() {
        emit senderBegin();
    });
    connect(sender, &FileSender::senderEnded, this, [&]() {
        emit senderEnded();
    });
    connect(sender, QOverload<int>::of(&FileSender::senderStatusUpdate), this, [&](int status) {
        emit senderStatusUpdate(status);
    });
}

void TransferSession::deleteConnection()
{

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

/* Slots */

void TransferSession::completed() { emit sessionCompleted(); }

void TransferSession::canceled()
{
    delete sender;
    emit sessionAborted();
}

