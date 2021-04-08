QT       += core gui network

android {
    QT   += androidextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    broadcaster.cpp \
    broadcastreceiver.cpp \
    filereceiver.cpp \
    filereceiveserver.cpp \
    filesender.cpp \
    filetransferpeer.cpp \
    main.cpp \
    mainwindow.cpp \
    onlinedevicesmodel.cpp \
    transfersession.cpp

HEADERS += \
    broadcaster.h \
    broadcastreceiver.h \
    filereceiver.h \
    filereceiveserver.h \
    filesender.h \
    filetransferpeer.h \
    mainwindow.h \
    onlinedevicesmodel.h \
    transfersession.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

ANDROID_ABIS = armeabi-v7a arm64-v8a
