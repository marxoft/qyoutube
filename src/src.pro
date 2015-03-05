TEMPLATE = lib

#DEFINES += QYOUTUBE_DEBUG

#unix:INSTALL_SRC_PREFIX = /opt

QT += network xml script
QT -= gui

TARGET = qyoutube
DESTDIR = ../lib

contains(MEEGO_EDITION,harmattan) {
    CONFIG += staticlib
    DEFINES += QYOUTUBE_STATIC_LIBRARY
} else {
    CONFIG += create_prl
    DEFINES += QYOUTUBE_LIBRARY
}

HEADERS += \
    activitiesmodel.h \
    activitiesrequest.h \
    authenticationrequest.h \
    channelsectionsmodel.h \
    channelsectionsrequest.h \
    channelsmodel.h \
    channelsrequest.h \
    guidecategoriesmodel.h \
    guidecategoriesrequest.h \
    i18nlanguagesmodel.h \
    i18nlanguagesrequest.h \
    i18nregionsmodel.h \
    i18nregionsrequest.h \
    json.h \
    model.h \
    model_p.h \
    playlistitemsmodel.h \
    playlistitemsrequest.h \
    playlistsmodel.h \
    playlistsrequest.h \
    request.h \
    request_p.h \
    searchmodel.h \
    searchrequest.h \
    streamsmodel.h \
    streamsrequest.h \
    subscriptionsmodel.h \
    subscriptionsrequest.h \
    subtitlesmodel.h \
    subtitlesrequest.h \
    videocategoriesmodel.h \
    videocategoriesrequest.h \
    videosmodel.h \
    videosrequest.h \
    urls.h

SOURCES += \
    activitiesmodel.cpp \
    activitiesrequest.cpp \
    authenticationrequest.cpp \
    channelsectionsmodel.cpp \
    channelsectionsrequest.cpp \
    channelsmodel.cpp \
    channelsrequest.cpp \
    guidecategoriesmodel.cpp \
    guidecategoriesrequest.cpp \
    i18nlanguagesmodel.cpp \
    i18nlanguagesrequest.cpp \
    i18nregionsmodel.cpp \
    i18nregionsrequest.cpp \
    json.cpp \
    playlistitemsmodel.cpp \
    playlistitemsrequest.cpp \
    playlistsmodel.cpp \
    playlistsrequest.cpp \
    model.cpp \
    request.cpp \
    searchmodel.cpp \
    searchrequest.cpp \
    streamsmodel.cpp \
    streamsrequest.cpp \
    subscriptionsmodel.cpp \
    subscriptionsrequest.cpp \
    subtitlesmodel.cpp \
    subtitlesrequest.cpp \
    videocategoriesmodel.cpp \
    videocategoriesrequest.cpp \
    videosmodel.cpp \
    videosrequest.cpp
    
headers.files += \
    activitiesmodel.h \
    activitiesrequest.h \
    authenticationrequest.h \
    channelsectionsmodel.h \
    channelsectionsrequest.h \
    channelsmodel.h \
    channelsrequest.h \
    guidecategoriesmodel.h \
    guidecategoriesrequest.h \
    i18nlanguagesmodel.h \
    i18nlanguagesrequest.h \
    i18nregionsmodel.h \
    i18nregionsrequest.h \
    model.h \
    playlistitemsmodel.h \
    playlistitemsrequest.h \
    playlistsmodel.h \
    playlistsrequest.h \
    request.h \
    searchmodel.h \
    searchrequest.h \
    streamsmodel.h \
    streamsrequest.h \
    subscriptionsmodel.h \
    subscriptionsrequest.h \
    subtitlesmodel.h \
    subtitlesrequest.h \
    videocategoriesmodel.h \
    videocategoriesrequest.h \
    videosmodel.h \
    videosrequest.h \
    urls.h

!isEmpty(INSTALL_SRC_PREFIX) {
    target.path = $$INSTALL_SRC_PREFIX/lib
    headers.path = $$INSTALL_SRC_PREFIX/include/qyoutube
} else {
    target.path = /usr/lib
    headers.path = /usr/include/qyoutube
}

INSTALLS += target headers
