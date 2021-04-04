#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

class FileReceiver : public QObject
{
    Q_OBJECT
public:
    explicit FileReceiver(QTcpSocket *socket, QObject *parent = nullptr);

private:
    QTcpSocket* tcpSocket;
    QString fileName;
    QFile *file;
    qint64 fileSize = -1;
    qint64 downloadedSize = 0;

    QByteArray fileBuffer;
    qint32 fileBufferSize = -1;

    int packetCount = 0;

    void sendData();
    void sendMeta();
    void metaParser(QByteArray& data);
    void writeData(QByteArray& data);



signals:

private slots:
    void socketConnected();
    void socketDisconnected();
    void readPacket();

};

#endif // FILERECEIVER_H
