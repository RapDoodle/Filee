#include "guicontroller.h"

GuiController::GuiController(QQmlContext *qmlContext, QObject *parent)
    : QObject(parent), context(qmlContext)
{
    nickname = Common::randomName();
    updateLocalIpAddress();
    // Start the file dialog at the user's desktop
    previousDir.setPath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    senderFileName = "Select a file...";
}

void GuiController::exec()
{
    broadcaster = new Broadcaster(nickname);
    broadcaster->startBroadcaster();

    onlineDevicesModel = new OnlineDevicesModel(broadcastReceiver);

    context->setContextProperty("_broadcaster", broadcaster);
    context->setContextProperty("_onlineDevicesList", onlineDevicesModel);
    context->setContextProperty("_broadcastReceiver", &broadcastReceiver);
    context->setContextProperty("_senderFileName", QVariant::fromValue(senderFileName));
    context->setContextProperty("_nickname", QVariant::fromValue(nickname));
}

void GuiController::setSenderFilePath(QString path)
{
    senderFilePath = path;
    #if defined (Q_OS_ANDROID)
    senderFileName = AndroidUtils::androidFileNameParser(path);
    #else
    senderFileName = QDir(senderFilePath).dirName();
    #endif
}

QString GuiController::getSenderFileName() const
{
    qDebug() << "Read: " << senderFileName;
    return senderFileName;
}

QString GuiController::getReceiverIpAddress() const { return receiverIp; }

void GuiController::setReceiverIpAddress(QString ip) { receiverIp = ip; }

QVariantList GuiController::getLocalIpAddress() const { return localIps; }

void GuiController::updateLocalIpAddress()
{
    QVariantList list;
    QList<QHostAddress> ips = Common::getLocalIpAddresses();
    for (auto ip : ips)
        list.append(ip.toString());
    localIps = list;
}
