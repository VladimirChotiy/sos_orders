QT -= gui
QT += core

CONFIG += c++11

TEMPLATE = lib

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $$PWD/inc/

HEADERS += \
    inc/mapsettings.h \
    inc/settings.h \
    inc/simplecrypt.h

SOURCES += \
    src/settings.cpp \
    src/simplecrypt.cpp

DESTDIR = $$PWD/../shared/lib/

headers.path = ../shared/inc/
headers.files = inc/settings.h \
                inc/mapsettings.h

INSTALLS += headers
