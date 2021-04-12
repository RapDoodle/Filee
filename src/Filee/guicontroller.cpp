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
    return senderFileName;
}

QString GuiController::getReceiverIpAddress() const { return receiverIp; }

void GuiController::setReceiverIpAddress(QString ip) { receiverIp = ip; }

void GuiController::setReceiverIpAddress(int row)
{
    QString newIp = onlineDevicesModel->getSelectedIp(row);
    if (receiverIp != newIp) {
        receiverIp = newIp;
        emit receiverIpChanged();
    }
}

QVariantList GuiController::getLocalIpAddress() const { return localIps; }

void GuiController::updateLocalIpAddress()
{
    QVariantList list;
    QList<QHostAddress> ips = Common::getLocalIpAddresses();
    for (auto ip : ips)
        list.append(ip.toString());
    localIps = list;
}

void GuiController::senderSend()
{
    if (senderFilePath.size() == 0) {
        MessageBox::messageBoxWarning("Please select a file to send.");
        return;
    }
    QHostAddress receiverAddress = QHostAddress(receiverIp);
    if (receiverAddress.isNull()) {
        MessageBox::messageBoxWarning("Please select or type in a file receiver.");
        return;
    }
    session = new TransferSession(senderFilePath, QHostAddress(receiverIp));
    connect(session, &TransferSession::senderBegin, this, [&]() {
        emit senderBegin();
    });
    connect(session, &TransferSession::senderEnded, this, [&]() {
        emit senderEnded();
    });
    connect(session, QOverload<int>::of(&TransferSession::senderStatusUpdate), this, [&](int status) {
        emit senderStatusUpdate(status);
    });
}

void GuiController::senderPause() { if (session) session->pause(); }

void GuiController::senderResume() { if (session) session->resume(); }

void GuiController::senderCancel() { if (session) session->cancel(); }
