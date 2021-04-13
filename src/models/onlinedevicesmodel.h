#ifndef ONLINEDEVICESMODEL_H
#define ONLINEDEVICESMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QHostAddress>
#include <QVector>
#include <QTime>
#include <QNetworkInterface>
#include <QTimer>

#include "./broadcast/broadcastreceiver.h"

enum class DeviceStatus : char
{
    Online  = 0x00,
    Offline = 0x01
};

struct Device
{
    QString name;
    QHostAddress address;
    DeviceStatus status;
    QTime lastReceivedTime;
};

class OnlineDevicesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Roles {
        NicknameRole = Qt::UserRole,
        IpRole,
        StatusRole
    };

    OnlineDevicesModel(BroadcastReceiver&);
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    QString getSelectedIp(int row);

private:
    BroadcastReceiver &receiver;
    QVector<Device> devices;
    QVector<QHostAddress> loopbackInterfaces;
    QTimer *statusCheckTimer = nullptr;



private slots:
    void statusUpdate(QHostAddress, QString);
    void onlineCheck();

};

#endif // ONLINEDEVICESMODEL_H
