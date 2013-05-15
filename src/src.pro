include(../common.pri)
include($$[QT_INSTALL_DATA]/mkspecs/dcpconfig.pri)
TEMPLATE = lib
CONFIG += qt \
    meegotouch \
    duicontrolpanel \
    plugin \
    link_pkgconfig \
    debug

PKGCONFIG += meegotouch libsocialweb-qt
INCLUDEPATH += /usr/include/libsocialweb

QT += dbus
target.path = $$DCP_APPLET_DIR


STYLE_HEADERS += 

HEADERS += $$STYLE_HEADERS \ 
    appletif.h \
    socialwebbrief.h \
    swservicelistwidget.h \
    swaccteditpage.h

SOURCES += \ 
    appletif.cpp \
    socialwebbrief.cpp \
    swservicelistwidget.cpp \
    swaccteditpage.cpp

INSTALLS += target
