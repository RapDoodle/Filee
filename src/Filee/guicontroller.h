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
public:
    explicit GuiController(QQmlContext*, QObject *parent = nullptr);
    Q_INVOKABLE void exec();

private:
    QQmlContext *context;
    Broadcaster broadcaster;
    BroadcastReceiver broadcastReceiver;
    OnlineDevicesModel *onlineDevicesModel;

signals:

};

#endif // GUICONTROLLER_H
