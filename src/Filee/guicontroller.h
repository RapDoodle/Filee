#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QObject>
#include <QQmlContext>
#include <QDebug>
#include <QFile>
#include <QVector>
#include <QDesktopServices>
#include <QFileDialog>
#include <QHostAddress>
#include <QStandardPaths>
#include <QApplication>

#include "./utils/common.h"
#include "./utils/messagebox.h"

#if defined (Q_OS_ANDROID)
#include "./utils/androidutils.h"
#endif

#include "./broadcast/broadcaster.h"
#include "./broadcast/broadcastreceiver.h"
#include "./transfer/sender/filesender.h"
#include "./transfer/receiver/filereceiveserver.h"
#include "./transfer/receiver/filereceiver.h"
#include "./models/onlinedevicesmodel.h"
#include "./transfer/sender/transfersession.h"

class GuiController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString qmlSenderFileName READ getSenderFileName)
    Q_PROPERTY(QString qmlSenderFilePath MEMBER senderFilePath)
    Q_PROPERTY(QVariantList qmlLocalIps READ getLocalIpAddress)
    Q_PROPERTY(QString qmlReceiverIp READ getReceiverIpAddress WRITE setReceiverIpAddress NOTIFY receiverIpChanged)
    Q_PROPERTY(QString qmlReceiverPeerIp READ getReceiverPeerIp NOTIFY receiverPeerIpChanged)
    Q_PROPERTY(QString qmlReceiverFilename READ getReceiverFilename NOTIFY receiverFilenameChanged)

public:
    explicit GuiController(QQmlContext*, QObject *parent = nullptr);
    Q_INVOKABLE void exec();
    Q_INVOKABLE void setSenderFilePath(QString);
    QString getSenderFileName() const;
    QString getReceiverIpAddress() const;
    void setReceiverIpAddress(QString);
    Q_INVOKABLE void setReceiverIpAddress(int);
    QVariantList getLocalIpAddress() const;
    void updateLocalIpAddress();
    QString getReceiverPeerIp();
    QString getReceiverFilename();

    Q_INVOKABLE void startBroadcast();
    Q_INVOKABLE void stopBroadcast();

    Q_INVOKABLE void senderSend();
    Q_INVOKABLE void senderPause();
    Q_INVOKABLE void senderResume();
    Q_INVOKABLE void senderCancel();

    Q_INVOKABLE void openReceivedFile();

private:
    QQmlContext *context;
    QApplication *app;

    Broadcaster *broadcaster;
    BroadcastReceiver broadcastReceiver;
    OnlineDevicesModel *onlineDevicesModel;

    QVariantList localIps;
    QString nickname;
    QDir previousDir;
    QString senderFilePath;
    QString senderFileName;
    QString receiverIp;

    QString receiverPeerIp;
    QString receiverFilename;
    QString receiverFilePath;

    FileReceiveServer *fileReceiveServer;
    TransferSession *session = nullptr;

signals:
    void qmlReceiverStatusUpdate(int);
    void qmlSenderStatusUpdate(int);
    void qmlSpeedUpdate(QString);
    void qmlSenderCancel();
    void qmlReceiverCancel();
    void qmlSenderComplete();
    void qmlReceiverComplete();

    void receiverIpChanged();
    void senderBegin();
    void senderEnded();
    void senderStatusUpdate(int status);

    void receiverBegin();
    void receiverEnded();
    void receiverStatusUpdate(int status);

    void receiverPeerIpChanged();
    void receiverFilenameChanged();

    void rateUpdate(QString rate);

};

#endif // GUICONTROLLER_H
