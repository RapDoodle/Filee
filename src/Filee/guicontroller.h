#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QObject>
#include <QQmlContext>
#include <QDebug>
#include <QFile>
#include <QVector>
#include <QFileDialog>
#include <QHostAddress>
#include <QStandardPaths>
#include <QApplication>

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

public:
    explicit GuiController(QQmlContext*, QObject *parent = nullptr);
    Q_INVOKABLE void exec();
    Q_INVOKABLE QString openFileDialog();
    Q_INVOKABLE void setSenderFilePath(QString);
    QString getSenderFileName() const;


private:
    QQmlContext *context;
    QApplication *app;
    Broadcaster broadcaster;
    BroadcastReceiver broadcastReceiver;
    OnlineDevicesModel *onlineDevicesModel;
    QDir previousDir;
    QString senderFilePath;
    QString senderFileName;




signals:

};

#endif // GUICONTROLLER_H
