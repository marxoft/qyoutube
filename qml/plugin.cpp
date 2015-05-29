#include "plugin.h"
#include "authenticationrequest.h"
#include "resourcesmodel.h"
#include "streamsmodel.h"
#include "subtitlesmodel.h"
#if QT_VERSION >= 0x050000
#include <qqml.h>
#else
#include <qdeclarative.h>
#endif

namespace QYouTube {

void Plugin::registerTypes(const char *uri) {
    Q_ASSERT(uri == QLatin1String("QYouTube"));

    qmlRegisterType<AuthenticationRequest>(uri, 1, 0, "AuthenticationRequest");
    qmlRegisterType<ResourcesModel>(uri, 1, 0, "ResourcesModel");
    qmlRegisterType<ResourcesRequest>(uri, 1, 0, "ResourcesRequest");
    qmlRegisterType<StreamsModel>(uri, 1, 0, "StreamsModel");
    qmlRegisterType<StreamsRequest>(uri, 1, 0, "StreamsRequest");
    qmlRegisterType<SubtitlesModel>(uri, 1, 0, "SubtitlesModel");
    qmlRegisterType<SubtitlesRequest>(uri, 1, 0, "SubtitlesRequest");
}

}

QML_DECLARE_TYPE(QYouTube::AuthenticationRequest)
QML_DECLARE_TYPE(QYouTube::ResourcesModel)
QML_DECLARE_TYPE(QYouTube::ResourcesRequest)
QML_DECLARE_TYPE(QYouTube::StreamsModel)
QML_DECLARE_TYPE(QYouTube::StreamsRequest)
QML_DECLARE_TYPE(QYouTube::SubtitlesModel)
QML_DECLARE_TYPE(QYouTube::SubtitlesRequest)
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qyoutubeplugin, QYouTube::Plugin)
#endif
