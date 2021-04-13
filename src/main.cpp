#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "guicontroller.h"

#if defined (Q_OS_ANDROID)
#include <QtAndroid>

bool requestAndroidPermissions()
{
    const QVector<QString> permissions({"android.permission.INTERNET",
                                        "android.permission.WRITE_EXTERNAL_STORAGE",
                                        "android.permission.READ_EXTERNAL_STORAGE",
                                        "android.permission.ACCESS_NETWORK_STATE"});

    for(const QString &permission : permissions) {
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
    #if defined (Q_OS_ANDROID)
    if(!requestAndroidPermissions())
        return -1;
    #endif

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();
    GuiController guiController(context);
    context->setContextProperty("_guiController", &guiController);
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
