#ifndef FILESENDER_H
#define FILESENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

#define BUFFER_SIZE 1024*1024  // 1MB

class FileSender : public QObject
{
    Q_OBJECT
public:
    explicit FileSender(QString filePath, QHostAddress receiverAddress, QObject *parent = nullptr);
    void resume();
    void pause();
    void cancel();

private:
    QTcpSocket* tcpSocket;
    QString fileDir;
    QFile *file;
    qint64 fileSize = -1;
    qint64 remainingSize = -1;

    QByteArray fileBuffer;
    qint32 fileBufferSize = -1;

    void sendData();
    void sendMeta();

signals:

private slots:
    void socketBytesWritten();
    void socketConnected();
    void socketDisconnected();

};

#endif // FILESENDER_H
