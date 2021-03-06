#ifndef FILESENDERSESSION_H
#define FILESENDERSESSION_H

#include <QObject>
#include <QHostAddress>

#include "filesender.h"

class FileSenderSession : public QObject
{
    Q_OBJECT

public:
    explicit FileSenderSession(QString filePath, QHostAddress receiverAddress, QObject *parent = nullptr);

    void endSession();
    void pause();
    void resume();
    void cancel();

signals:
    void senderStatusUpdate(int);
    void senderBegin();
    void senderEnded();
    void sessionCompleted();
    void sessionAborted();
    void rateUpdate(QString);

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
    void overloadedHandler();
    void completed();
    void canceled();

};

#endif // FILESENDERSESSION_H
