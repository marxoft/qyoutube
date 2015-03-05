#include "plugin.h"
#include "activitiesmodel.h"
#include "authenticationrequest.h"
#include "channelsectionsmodel.h"
#include "channelsmodel.h"
#include "guidecategoriesmodel.h"
#include "i18nlanguagesmodel.h"
#include "i18nregionsmodel.h"
#include "playlistitemsmodel.h"
#include "playlistsmodel.h"
#include "searchmodel.h"
#include "streamsmodel.h"
#include "subscriptionsmodel.h"
#include "subtitlesmodel.h"
#include "videocategoriesmodel.h"
#include "videosmodel.h"
#if QT_VERSION >= 0x050000
#include <qqml.h>
#else
#include <qdeclarative.h>
#endif

namespace QYouTube {

void Plugin::registerTypes(const char *uri) {
    Q_ASSERT(uri == QLatin1String("QYouTube"));

    qmlRegisterType<ActivitiesModel>(uri, 1, 0, "ActivitiesModel");
    qmlRegisterType<ActivitiesRequest>(uri, 1, 0, "ActivitiesRequest");
    qmlRegisterType<AuthenticationRequest>(uri, 1, 0, "AuthenticationRequest");
    qmlRegisterType<ChannelSectionsModel>(uri, 1, 0, "ChannelSectionsModel");
    qmlRegisterType<ChannelSectionsRequest>(uri, 1, 0, "ChannelSectionsRequest");
    qmlRegisterType<ChannelsModel>(uri, 1, 0, "ChannelsModel");
    qmlRegisterType<ChannelsRequest>(uri, 1, 0, "ChannelsRequest");
    qmlRegisterType<GuideCategoriesModel>(uri, 1, 0, "GuideCategoriesModel");
    qmlRegisterType<GuideCategoriesRequest>(uri, 1, 0, "GuideCategoriesRequest");
    qmlRegisterType<I18nLanguagesModel>(uri, 1, 0, "I18nLanguagesModel");
    qmlRegisterType<I18nLanguagesRequest>(uri, 1, 0, "I18nLanguagesRequest");
    qmlRegisterType<I18nRegionsModel>(uri, 1, 0, "I18nRegionsModel");
    qmlRegisterType<I18nRegionsRequest>(uri, 1, 0, "I18nRegionsRequest");
    qmlRegisterType<PlaylistItemsModel>(uri, 1, 0, "PlaylistItemsModel");
    qmlRegisterType<PlaylistItemsRequest>(uri, 1, 0, "PlaylistItemsRequest");
    qmlRegisterType<PlaylistsModel>(uri, 1, 0, "PlaylistsModel");
    qmlRegisterType<PlaylistsRequest>(uri, 1, 0, "PlaylistsRequest");
    qmlRegisterType<SearchModel>(uri, 1, 0, "SearchModel");
    qmlRegisterType<SearchRequest>(uri, 1, 0, "SearchRequest");
    qmlRegisterType<StreamsModel>(uri, 1, 0, "StreamsModel");
    qmlRegisterType<StreamsRequest>(uri, 1, 0, "StreamsRequest");
    qmlRegisterType<SubscriptionsModel>(uri, 1, 0, "SubscriptionsModel");
    qmlRegisterType<SubscriptionsRequest>(uri, 1, 0, "SubscriptionsRequest");
    qmlRegisterType<SubtitlesModel>(uri, 1, 0, "SubtitlesModel");
    qmlRegisterType<SubtitlesRequest>(uri, 1, 0, "SubtitlesRequest");
    qmlRegisterType<VideoCategoriesModel>(uri, 1, 0, "VideoCategoriesModel");
    qmlRegisterType<VideoCategoriesRequest>(uri, 1, 0, "VideoCategoriesRequest");
    qmlRegisterType<VideosModel>(uri, 1, 0, "VideosModel");
    qmlRegisterType<VideosRequest>(uri, 1, 0, "VideosRequest");
}

}

QML_DECLARE_TYPE(QYouTube::ActivitiesModel)
QML_DECLARE_TYPE(QYouTube::ActivitiesRequest)
QML_DECLARE_TYPE(QYouTube::AuthenticationRequest)
QML_DECLARE_TYPE(QYouTube::ChannelSectionsModel)
QML_DECLARE_TYPE(QYouTube::ChannelSectionsRequest)
QML_DECLARE_TYPE(QYouTube::ChannelsModel)
QML_DECLARE_TYPE(QYouTube::ChannelsRequest)
QML_DECLARE_TYPE(QYouTube::GuideCategoriesModel)
QML_DECLARE_TYPE(QYouTube::GuideCategoriesRequest)
QML_DECLARE_TYPE(QYouTube::I18nLanguagesModel)
QML_DECLARE_TYPE(QYouTube::I18nLanguagesRequest)
QML_DECLARE_TYPE(QYouTube::I18nRegionsModel)
QML_DECLARE_TYPE(QYouTube::I18nRegionsRequest)
QML_DECLARE_TYPE(QYouTube::PlaylistItemsModel)
QML_DECLARE_TYPE(QYouTube::PlaylistItemsRequest)
QML_DECLARE_TYPE(QYouTube::PlaylistsModel)
QML_DECLARE_TYPE(QYouTube::PlaylistsRequest)
QML_DECLARE_TYPE(QYouTube::SearchModel)
QML_DECLARE_TYPE(QYouTube::SearchRequest)
QML_DECLARE_TYPE(QYouTube::StreamsModel)
QML_DECLARE_TYPE(QYouTube::StreamsRequest)
QML_DECLARE_TYPE(QYouTube::SubscriptionsModel)
QML_DECLARE_TYPE(QYouTube::SubscriptionsRequest)
QML_DECLARE_TYPE(QYouTube::SubtitlesModel)
QML_DECLARE_TYPE(QYouTube::SubtitlesRequest)
QML_DECLARE_TYPE(QYouTube::VideoCategoriesModel)
QML_DECLARE_TYPE(QYouTube::VideoCategoriesRequest)
QML_DECLARE_TYPE(QYouTube::VideosModel)
QML_DECLARE_TYPE(QYouTube::VideosRequest)

Q_EXPORT_PLUGIN2(qyoutubeplugin, QYouTube::Plugin)
