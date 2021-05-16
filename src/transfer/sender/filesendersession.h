#ifndef FILESENDERSESSION_H
#define FILESENDERSESSION_H

#include <QObject>
#include <QHostAddress>

#include "filesender.h"
#include "filesendersecure.h"

class FileSenderSession : public QObject
{
    Q_OBJECT

public:
    explicit FileSenderSession(QString filePath, QHostAddress receiverAddress, bool secure, QObject *parent = nullptr);

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
    bool useSecure = false;
    FileSender *sender = nullptr;
    int attempt = 0;
    bool restart = false;
    bool secure = false;

    void transfer();
    void deleteConnection();

private slots:
    void overloadedHandler();
    void completed();
    void canceled();

};

#endif // FILESENDERSESSION_H
