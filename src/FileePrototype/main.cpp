#include "mainwindow.h"

#include <QApplication>
#if defined (Q_OS_ANDROID)
#include <QtAndroid>

bool requestAndroidPermissions()
{
    const QVector<QString> permissions({"android.permission.INTERNET",
                                        "android.permission.WRITE_EXTERNAL_STORAGE",
                                        "android.permission.READ_EXTERNAL_STORAGE"});

    for(const QString &permission : permissions){
        auto result = QtAndroid::checkPermission(permission);
        if(result == QtAndroid::PermissionResult::Denied) {
            auto resultHash = QtAndroid::requestPermissionsSync(QStringList({permission}));
            if(resultHash[permission] == QtAndroid::PermissionResult::Denied)
                return false;
        }
    }

    return true;
}
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    #if defined (Q_OS_ANDROID)
    if(!requestAndroidPermissions())
        return -1;
    #endif

    return a.exec();
}
