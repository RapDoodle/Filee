#include "guicontroller.h"
#include "./utils/utils.h"

GuiController::GuiController(QQmlContext *qmlContext, QObject *parent)
    : QObject(parent), context(qmlContext)
{
    // Start the file dialog at the user's desktop
    previousDir.setPath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    senderFileName = "heelo.xt";
}

void GuiController::exec()
{
    broadcaster.startBroadcaster();

    onlineDevicesModel = new OnlineDevicesModel(broadcastReceiver);

    context->setContextProperty("_broadcaster", &broadcaster);
    context->setContextProperty("_onlineDevicesList", onlineDevicesModel);
    context->setContextProperty("_broadcastReceiver", &broadcastReceiver);
    context->setContextProperty("_senderFileName", QVariant::fromValue(senderFileName));
}

QString GuiController::openFileDialog()
{
//    senderFilePath = QFileDialog::getOpenFileName(app, tr("Open file"), previousDir.absolutePath());
    previousDir.setPath(senderFilePath);

    return senderFileName;
}

void GuiController::setSenderFilePath(QString path)
{
    qDebug() << path;
    senderFilePath = path;
    #if defined (Q_OS_ANDROID)
    senderFileName = Utils::androidFileNameParser(path);
    #else
    senderFileName = QDir(senderFilePath).dirName();
#endif
}

QString GuiController::getSenderFileName() const
{
    qDebug() << "Read: " << senderFileName;
    return senderFileName;
}
