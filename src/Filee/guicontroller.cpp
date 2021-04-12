#include "guicontroller.h"

GuiController::GuiController(QQmlContext *qmlContext, QObject *parent)
    : QObject(parent), context(qmlContext)
{
    nickname = Common::randomName();
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
