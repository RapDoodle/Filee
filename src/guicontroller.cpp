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

    fileReceiveServer = new FileReceiveServer();
    connect(fileReceiveServer, &FileReceiveServer::receiverBegin, this, [&](QString sender,
            QString filename, QString filePath) {
        receiverPeerIp = sender;
        receiverFilename = filename;
        receiverFilePath = filePath;
        emit receiverPeerIpChanged();
        emit receiverFilenameChanged();
        emit receiverBegin();
    });
    connect(fileReceiveServer, &FileReceiveServer::receiverEnded, this, [&]() {
        emit receiverEnded();
    });
    connect(fileReceiveServer, &FileReceiveServer::receiverTerminated, this, [&]() {
        emit receiverTerminated();
    });
    connect(fileReceiveServer, QOverload<int>::of(&FileReceiveServer::receiverStatusUpdate),
            this, [&](int status) {
        emit receiverStatusUpdate(status);
    });
    connect(fileReceiveServer, QOverload<QString>::of(&FileReceiveServer::rateUpdate),
            this, [&](QString rate) {
        emit rateUpdate(rate);
    });

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
    session = new FileSenderSession(senderFilePath, QHostAddress(receiverIp));
    connect(session, &FileSenderSession::senderBegin, this, [&]() {
        emit senderBegin();
    });
    connect(session, &FileSenderSession::senderEnded, this, [&]() {
        emit senderEnded();
    });
    connect(session, QOverload<int>::of(&FileSenderSession::senderStatusUpdate), this, [&](int status) {
        emit senderStatusUpdate(status);
    });
    connect(session, QOverload<QString>::of(&FileSenderSession::rateUpdate), this, [&](QString rate) {
        emit rateUpdate(rate);
    });
}

void GuiController::openReceivedFile()
{
    if (receiverFilePath.size() > 0) {
//        QProcess::startDetached(receiverFilePath, parameters);
        QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/Filee/" + receiverFilename;
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(path))) {
            MessageBox::messageBoxCritical("Permission denied. Unable to open the received file. But the file is stored in "
                                           + QUrl::fromLocalFile(path).toString());
        }
    }
}

void GuiController::senderPause() { if (session) session->pause(); }

void GuiController::senderResume() { if (session) session->resume(); }

void GuiController::senderCancel() { if (session) session->cancel(); }

void GuiController::receiverPause() { fileReceiveServer->pause(); }

void GuiController::receiverResume() { fileReceiveServer->resume(); }

void GuiController::receiverCancel() { fileReceiveServer->cancel(); }

void GuiController::startBroadcast() { broadcaster->startBroadcaster(); }

void GuiController::stopBroadcast() { broadcaster->stopBroadcaster(); }

QString GuiController::getReceiverPeerIp() { return receiverPeerIp; }

QString GuiController::getReceiverFilename() { return receiverFilename; }
