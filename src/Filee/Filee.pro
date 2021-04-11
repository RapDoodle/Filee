QT       += core quick gui network

android {
    QT   += androidextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        broadcast/broadcaster.cpp \
        broadcast/broadcastreceiver.cpp \
        main.cpp \
        models/onlinedevicesmodel.cpp \
        transfer/filetransferpeer.cpp \
        transfer/receiver/filereceiver.cpp \
        transfer/receiver/filereceiveserver.cpp \
        transfer/sender/filesender.cpp \
        transfer/sender/transfersession.cpp \
        utils/utils.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a

HEADERS += \
    broadcast/broadcaster.h \
    broadcast/broadcastreceiver.h \
    models/onlinedevicesmodel.h \
    transfer/filetransferpeer.h \
    transfer/receiver/filereceiver.h \
    transfer/receiver/filereceiveserver.h \
    transfer/sender/filesender.h \
    transfer/sender/transfersession.h \
    utils/utils.h
