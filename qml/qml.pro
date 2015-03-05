TEMPLATE = lib
TARGET = qyoutubeplugin
CONFIG += qt plugin link_prl
LIBS += -L../lib -lqyoutube
PKGCONFIG = libqyoutube
INCLUDEPATH += ../src

lessThan(QT_MAJOR_VERSION, 5) {
    QT += declarative
} else {
    QT += qml
}

HEADERS += \
    plugin.h

SOURCES += \
    plugin.cpp

qml.files = qmldir

!isEmpty(INSTALL_QML_PREFIX) {
    qml.path = $$INSTALL_QML_PREFIX/lib/qt4/imports/QYouTube
    target.path = $$INSTALL_QML_PREFIX/lib/qt4/imports/QYouTube
} else {
    qml.path = $$QT_INSTALL_IMPORTS/QYouTube
    target.path = $$QT_INSTALL_IMPORTS/QYouTube
}

INSTALLS += qml target
