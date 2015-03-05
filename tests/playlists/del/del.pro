TEMPLATE = app
TARGET = playlists-del
INSTALLS += target

INCLUDEPATH += ../../../src
LIBS += -L../../../lib -lqyoutube
SOURCES += main.cpp

unix {
    target.path = /opt/qyoutube/bin
}
