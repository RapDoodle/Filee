#include "filesendersession.h"

FileSenderSession::FileSenderSession(QString filePath, QHostAddress receiverAddress, bool useSecure, QObject *parent) :
    QObject(parent), fileDir(filePath), address(receiverAddress), secure(useSecure)
{
    bufferSize = 1024 * 8192;

    #if defined (Q_OS_ANDROID)
    bufferSize = 1024 * 8192;
    #endif

    transfer();
}

void FileSenderSession::transfer()
{
    if (secure) {
        sender = new FileSenderSecure(fileDir, address, bufferSize);
        connect(sender, &FileSenderSecure::restartRequest, this, &FileSenderSession::overloadedHandler);
        connect(sender, &FileSenderSecure::senderBegin, this, [&]() {
            emit senderBegin();
        });
        connect(sender, &FileSenderSecure::senderEnded, this, [&]() {
            emit senderEnded();
        });
        connect(sender, QOverload<int>::of(&FileSenderSecure::senderStatusUpdate), this, [&](int status) {
            emit senderStatusUpdate(status);
        });
        connect(sender, QOverload<QString>::of(&FileSenderSecure::rateUpdate), this, [&](QString rate) {
            emit rateUpdate(rate);
        });
    } else {
        sender = new FileSender(fileDir, address, bufferSize);
        connect(sender, &FileSender::restartRequest, this, &FileSenderSession::overloadedHandler);
        connect(sender, &FileSender::senderBegin, this, [&]() {
            emit senderBegin();
        });
        connect(sender, &FileSender::senderEnded, this, [&]() {
            emit senderEnded();
        });
        connect(sender, QOverload<int>::of(&FileSender::senderStatusUpdate), this, [&](int status) {
            emit senderStatusUpdate(status);
        });
        connect(sender, QOverload<QString>::of(&FileSender::rateUpdate), this, [&](QString rate) {
            emit rateUpdate(rate);
        });
    }

}

void FileSenderSession::deleteConnection()
{

}

void FileSenderSession::overloadedHandler()
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

void FileSenderSession::endSession()
{
    deleteConnection();
    delete sender;
    emit sessionAborted();
}

void FileSenderSession::pause()
{
    if (sender)
        sender->pause();
}

void FileSenderSession::resume()
{
    if (sender)
        sender->resume();
}

void FileSenderSession::cancel()
{
    if (sender)
        sender->cancel();
}

/* Slots */

void FileSenderSession::completed() { emit sessionCompleted(); }

void FileSenderSession::canceled()
{
    delete sender;
    emit sessionAborted();
}

