TEMPLATE = lib

#DEFINES += QYOUTUBE_DEBUG
#DEFINES += QYOUTUBE_STATIC_LIBRARY

QT += network xml script
QT -= gui

TARGET = qyoutube
DESTDIR = ../lib

HEADERS += \
    authenticationrequest.h \
    json.h \
    model.h \
    model_p.h \
    qyoutube_global.h \
    request.h \
    request_p.h \
    resourcesmodel.h \
    resourcesrequest.h \
    streamsmodel.h \
    streamsrequest.h \
    subtitlesmodel.h \
    subtitlesrequest.h \
    urls.h

SOURCES += \
    authenticationrequest.cpp \
    json.cpp \
    model.cpp \
    request.cpp \
    resourcesmodel.cpp \
    resourcesrequest.cpp \
    streamsmodel.cpp \
    streamsrequest.cpp \
    subtitlesmodel.cpp \
    subtitlesrequest.cpp
    
headers.files += \
    authenticationrequest.h \
    model.h \
    qyoutube_global.h \
    request.h \
    resourcesmodel.h \
    resourcesrequest.h \
    streamsmodel.h \
    streamsrequest.h \
    subtitlesmodel.h \
    subtitlesrequest.h \
    urls.h
    
symbian {
    TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.EPOCHEAPSIZE = 0x20000 0x8000000
    TARGET.EPOCSTACKSIZE = 0x14000
}

contains(DEFINES,QYOUTUBE_STATIC_LIBRARY) {
    CONFIG += staticlib
} else {
    CONFIG += create_prl
    INSTALLS += target headers

    !isEmpty(INSTALL_SRC_PREFIX) {
        target.path = $$INSTALL_SRC_PREFIX/lib
        headers.path = $$INSTALL_SRC_PREFIX/include/qyoutube
    } else {
        target.path = /usr/lib
        headers.path = /usr/include/qyoutube
    }
}
