#include "guicontroller.h"

GuiController::GuiController(QQmlContext *qmlContext, QObject *parent)
    : QObject(parent), context(qmlContext)
{

}

void GuiController::exec()
{
    broadcaster.startBroadcaster();

    onlineDevicesModel = new OnlineDevicesModel(broadcastReceiver);

    context->setContextProperty("_broadcaster", &broadcaster);
    context->setContextProperty("_onlineDevicesList", onlineDevicesModel);
    context->setContextProperty("_broadcastReceiver", &broadcastReceiver);
}
