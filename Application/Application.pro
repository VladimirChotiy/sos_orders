QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/ConnectionManager.cpp \
    src/DBProcessor.cpp \
    src/chooseengineer.cpp \
    src/clDBMainQueryModel.cpp \
    src/clDBReqInserter.cpp \
    src/editotderstatus.cpp \
    main.cpp \
    src/mainwindow.cpp \
    src/querymainrequests.cpp \
    src/uconnect_db.cpp \
    src/uregisternewrequest.cpp \
    src/ueditrequest.cpp

INCLUDEPATH += $$PWD/inc \
            $$PWD/../shared/inc

HEADERS += \
    inc/ConnectionManager.h \
    inc/DBProcessor.h \
    inc/clDBMainQueryModel.h \
    inc/clDBReqInserter.h \
    inc/dbtypes.h \
    inc/chooseengineer.h \
    inc/editotderstatus.h \
    inc/mainwindow.h \
    inc/querymainrequests.h \
    inc/uconnect_db.h \
    inc/uregisternewrequest.h \
    inc/ueditrequest.h

FORMS += \
    ui/ueditrequest.ui \
    ui/chooseengineer.ui \
    ui/editotderstatus.ui \
    ui/mainwindow.ui \
    ui/uconnect_db.ui \
    ui/uregisternewrequest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res.qrc

LIBS += -L$$PWD/../shared/lib/ -lStoreSettings

#win32:RC_ICONS = $$PWD/Icons/three-bars.ico
