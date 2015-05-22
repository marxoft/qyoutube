TEMPLATE = lib
TARGET = qyoutubeplugin
CONFIG += qt plugin
LIBS += -L../lib -lqyoutube

contains(MEEGO_EDITION,harmattan) {
    CONFIG += link_pkgconfig
    INSTALL_QML_PREFIX = /usr
} else {
    CONFIG += link_prl
    PKGCONFIG = libqyoutube
}

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
} else:lessThan(QT_MAJOR_VERSION, 5) {
    qml.path = $$[QT_INSTALL_IMPORTS]/QYouTube
    target.path = $$[QT_INSTALL_IMPORTS]/QYouTube
} else {
    qml.path = $$[QT_INSTALL_QML]/QYouTube
    target.path = $$[QT_INSTALL_QML]/QYouTube
}

INSTALLS += qml target
