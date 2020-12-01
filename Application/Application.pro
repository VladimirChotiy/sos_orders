QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/cldbaccesslevel.cpp \
    src/cldbprotocolquerymodel.cpp \
    src/uiprotocolview.cpp \
    src/uisetcost.cpp \
    src/uichangestatus.cpp \
    src/uichooseengineer.cpp \
    src/pgrequestwizard.cpp \
    src/pgobjectwizard.cpp \
    src/pgpersonwizard.cpp \
    src/ConnectionManager.cpp \
    src/DBProcessor.cpp \
    src/clDBMainQueryModel.cpp \
    src/clDBReqInserter.cpp \
    main.cpp \
    src/mainwindow.cpp \
    src/uconnect_db.cpp \
    src/ueditrequest.cpp \
    src/uiaddnewwizard.cpp

INCLUDEPATH += $$PWD/inc \
            $$PWD/../shared/inc

HEADERS += \
    inc/ConnectionManager.h \
    inc/DBProcessor.h \
    inc/clDBMainQueryModel.h \
    inc/clDBReqInserter.h \
    inc/cldbaccesslevel.h \
    inc/cldbprotocolquerymodel.h \
    inc/dbtypes.h \
    inc/mainwindow.h \
    inc/pgobjectwizard.h \
    inc/pgpersonwizard.h \
    inc/pgrequestwizard.h \
    inc/uconnect_db.h \
    inc/ueditrequest.h \
    inc/uiaddnewwizard.h \
    inc/uichangestatus.h \
    inc/uichooseengineer.h \
    inc/uiprotocolview.h \
    inc/uisetcost.h

FORMS += \
    ui/uiprotocolview.ui \
    ui/uisetcost.ui \
    ui/uichangestatus.ui \
    ui/uichooseengineer.ui \
    ui/pgrequestwizard.ui \
    ui/pgobjectwizard.ui \
    ui/pgpersonwizard.ui \
    ui/ueditrequest.ui \
    ui/mainwindow.ui \
    ui/uconnect_db.ui \
    ui/uiaddnewwizard.ui

## Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Res.qrc

LIBS += -L$$PWD/../shared/lib/ -lStoreSettings

win32:RC_ICONS = $$PWD/Icons/three-bars.ico

CONFIG(release, debug|release) {
    DESTDIR = $$OUT_PWD/../../SOSRequsetRelease
}

MOC_DIR = ../common/build/moc
RCC_DIR = ../common/build/rcc
UI_DIR = ../common/build/ui
unix:OBJECTS_DIR = ../common/build/o/unix
win32:OBJECTS_DIR = ../common/build/o/win32
macx:OBJECTS_DIR = ../common/build/o/mac

CONFIG(release, debug|release) {
    QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/../../SOSRequsetRelease
}
