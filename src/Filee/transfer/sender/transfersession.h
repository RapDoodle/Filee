#ifndef TRANSFERSESSION_H
#define TRANSFERSESSION_H

#include <QObject>
#include <QHostAddress>

#include "filesender.h"

class TransferSession : public QObject
{
    Q_OBJECT

public:
    explicit TransferSession(QString filePath, QHostAddress receiverAddress, QObject *parent = nullptr);

    void endSession();
    void pause();
    void resume();
    void cancel();

signals:
    void progressUpdate(int);
    void sessionCompleted();
    void sessionAborted();

private:
    QString fileDir;
    QHostAddress address;
    qint64 bufferSize;
    FileSender *sender = nullptr;
    int attempt = 0;
    bool restart = false;

    void transfer();
    void deleteConnection();

private slots:
    void senderProgressChanged(int);
    void overloadedHandler();
    void completed();
    void canceled();

};

#endif // TRANSFERSESSION_H
