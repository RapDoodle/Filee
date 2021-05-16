#ifndef FILERECEIVESERVER_H
#define FILERECEIVESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>
#include <QFile>

#include "filereceiver.h"
#include "filereceiversecure.h"
#include "../../include/sslserver.h"
#include "../../include/sslsocket.h"

#include "../../utils/messagebox.h"

#define CERTNAME "cert.pem"
#define PRIVATEKEYNAME "private.pem"

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
    SslServer *sslServer = nullptr;

signals:
    void receiverBegin(QString sender, QString filename, QString filePath);
    void receiverEnded();
    void receiverTerminated();
    void receiverStatusUpdate(int);
    void rateUpdate(QString);

private slots:
    void newConnection();
    void newSecureConnection();

};

#endif // FILERECEIVESERVER_H
