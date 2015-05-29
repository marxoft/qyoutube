TEMPLATE = lib
TARGET = qyoutubeplugin
CONFIG += qt plugin
LIBS += -L../lib -lqyoutube

lessThan(QT_MAJOR_VERSION, 5) {
    QT += declarative
} else {
    QT += qml
}

INCLUDEPATH += ../src

HEADERS += \
    plugin.h

SOURCES += \
    plugin.cpp
    
contains(QYOUTUBE_STATIC_LIBRARY) {
    CONFIG += link_pkgconfig
} else {
    CONFIG += link_prl
    PKGCONFIG = libqyoutube
}

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
