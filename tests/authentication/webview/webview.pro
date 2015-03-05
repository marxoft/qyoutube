TEMPLATE = app
TARGET = authentication-webview
INSTALLS += target

DEFINES += QYOUTUBE_DEBUG
QT += webkit
INCLUDEPATH += ../../../src
LIBS += -L../../../lib -lqyoutube
HEADERS += webview.h
SOURCES += main.cpp webview.cpp

unix {
    target.path = /opt/qyoutube/bin
}
