#ifndef FILERECEIVESERVER_H
#define FILERECEIVESERVER_H

#include <QObject>
#include <QTcpServer>

#include "filereceiver.h"

class FileReceiveServer : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiveServer(QObject *parent = nullptr);
    void pause();
    void resume();
    void cancel();

private:
    QVector<FileReceiver*> receivers;
    QTcpServer *tcpServer = nullptr;

signals:
    void receiverBegin(QString sender, QString filename, QString filePath);
    void receiverEnded();
    void receiverTerminated();
    void receiverStatusUpdate(int);
    void rateUpdate(QString);

private slots:
    void newConnection();

};

#endif // FILERECEIVESERVER_H
