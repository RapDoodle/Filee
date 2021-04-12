#include "onlinedevicesmodel.h"

OnlineDevicesModel::OnlineDevicesModel(BroadcastReceiver &broadcastReceiver)
    : receiver(broadcastReceiver)
{
    connect(&broadcastReceiver,
            QOverload<QHostAddress, QString>::of(&BroadcastReceiver::broadcastMessageReceived),
            this,
            QOverload<QHostAddress, QString>::of(&OnlineDevicesModel::statusUpdate));

    // Retrive all loop back interfaces
    QList<QHostAddress> allAddresses = QNetworkInterface::allAddresses();

    for (int i = 0; i < allAddresses.count(); i++)
    {
        if(!allAddresses[i].isLoopback())
            if (allAddresses[i].protocol() == QAbstractSocket::IPv4Protocol)
                loopbackInterfaces.push_back(allAddresses[i]);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&OnlineDevicesModel::onlineCheck));
    timer->start(1000);
}

int OnlineDevicesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return devices.size();
}

int OnlineDevicesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant OnlineDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == NicknameRole) {
        return devices.at(index.row()).name;
    } else if (role == StatusRole) {
        switch(devices.at(index.row()).status) {
        case DeviceStatus::Online: return "Online";
        case DeviceStatus::Offline: return "Offline";
        default: return "Unknown";
        }
    } else if (role == IpRole) {
        return devices.at(index.row()).address.toString();
    } else {
        return QVariant();
    }
}

QVariant OnlineDevicesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return "IP";
            case 1: return "Nickname";
            case 2: return "Status";
            default: return "Unknown";
            }
        }
    }

    return QVariant();
}

bool OnlineDevicesModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    endInsertRows();
    return true;
}

void OnlineDevicesModel::statusUpdate(QHostAddress senderAddress, QString name)
{

    // Check whether it is a loopback interface
    for (int i = 0; i < loopbackInterfaces.size(); i++)
        if (senderAddress.toString() == loopbackInterfaces.at(i).toString())
            return;

    // Check if it already exists
    for (int i = 0; i < devices.size(); i++) {
        Device device = devices.at(i);

        if (senderAddress.toString() != device.address.toString())
            continue;

        // When the device is already in the list
        // Set status to online if it is offline
        if (device.status == DeviceStatus::Offline) {
            // Set to online
            devices[i].status = DeviceStatus::Online;
            emit dataChanged(index(i, 2), index(i, 2));

            // Update device name (may due to restart)
            if (device.name != name) {
                devices[i].name = name;
                emit dataChanged(index(i, 0), index(i, 0));
            }
        }

        // Update lastReceivedTime to current time
        devices[i].lastReceivedTime = QTime::currentTime();

        return;
    }

    // If not present, create a new one
    Device newDevice;
    newDevice.name = name;
    newDevice.address = senderAddress;
    newDevice.lastReceivedTime = QTime::currentTime();
    newDevice.status = DeviceStatus::Online;

    devices.push_back(newDevice);
    insertRows(devices.size() - 1, 1, QModelIndex());
}

void OnlineDevicesModel::OnlineDevicesModel::onlineCheck()
{
    QTime currentTime = QTime::currentTime();
    for (int i = 0; i < devices.size(); i++) {
        Device currentDevice = devices.at(i);
        if (currentDevice.status == DeviceStatus::Offline)
            continue;
        qint64 timeInterval = currentDevice.lastReceivedTime.secsTo(currentTime);
        if (timeInterval > 5) {
            // Set status to offline
            devices[i].status = DeviceStatus::Offline;
            emit dataChanged(index(i, 0), index(i, 2));
        }
    }
}

QHash<int, QByteArray> OnlineDevicesModel::roleNames() const
{
    static QHash<int, QByteArray> mapping {
        {NicknameRole, "nickname"},
        {StatusRole, "status"},
        {IpRole, "ip"}
    };
    return mapping;
}

QString OnlineDevicesModel::getSelectedIp(int row) {
    return devices.at(row).address.toString();
}
