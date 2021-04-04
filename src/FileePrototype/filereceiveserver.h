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

private:
    QVector<FileReceiver*> receivers;
    QTcpServer *tcpServer = nullptr;


signals:

private slots:
    void newConnection();

};

#endif // FILERECEIVESERVER_H
