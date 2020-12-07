QT -= gui
QT += core

CONFIG += c++11

TEMPLATE = lib

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/inc/

HEADERS += \
    inc/settings.h \
    inc/simplecrypt.h

SOURCES += \
    src/settings.cpp \
    src/simplecrypt.cpp

DESTDIR = $$PWD/../shared/lib/

headers.path = ../shared/inc/
headers.files = inc/settings.h

INSTALLS += headers

CONFIG(release, debug|release) {
    DESTDIR = $$OUT_PWD/../../SOSRequsetRelease
}
