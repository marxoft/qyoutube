/*
 * Copyright (C) 2015 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resourcesrequest.h"
#include "request_p.h"
#include "urls.h"

namespace QYouTube {

/*!
    \class ResourcesRequest
    \brief Handles requests for YouTube resources.
    
    \ingroup requests
    
    The ResourcesRequest class is used for making requests to the YouTube Data API that concern 
    YouTube resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    ResourcesRequest request;
    request.get("/video/VIDEO_ID");
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == ResourcesRequest::Ready) {            
            QMapIterator<QString, QVariant> iterator(request.result().toMap());
            
            while (iterator.hasNext()) {
                iterator.next();
                qDebug() << iterator.key() << "=" << iterator.value();
            }
        }
        else {
            qDebug() << request.errorString();
        }
    }
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QYouTube 1.0
    
    ResourcesRequest {
        id: request

        onFinished: {
            if (status == ResourcesRequest.Ready) {
                for (var k in result) {
                    console.log(att + " = " + result[k]);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: get("/video/VIDEO_ID")
    }
    \endcode
    
    For more details about YouTube resources, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs">here</a>.
*/
ResourcesRequest::ResourcesRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a list of YouTube resources from \a resourcePath.
    
    For example, to search videos:
    
    \code
    ResourcesRequest request;
    QVariantMap params;
    params["maxResults"] = 10;
    params["order"] = "date";
    params["q"] = "Qt";
    params["type"] = "video";
    request.list("/videos", QStringList() << "snippet", QVariantMap(), params);
    \endcode
*/
void ResourcesRequest::list(const QString &resourcePath, const QStringList &part, const QVariantMap &filters,
                            const QVariantMap &params) {
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("part", part.join(","));
    
    if (!filters.isEmpty()) {
        addUrlQueryItems(&query, filters);
    }
    
    if (!params.isEmpty()) {
        addUrlQueryItems(&query, params);
    }
    
    u.setQuery(query);
#else
    u.addQueryItem("part", part.join(","));
    
    if (!filters.isEmpty()) {
        addUrlQueryItems(&u, filters);
    }
    
    if (!params.isEmpty()) {
        addUrlQueryItems(&u, params);
    }
#endif
    setUrl(u);
    get();
}

/*!
    \brief Inserts a new YouTube resource.
    
    For example, to insert a new playlist on behalf of the authenticated user:
    
    \code
    ResourcesRequest request;
    QVariantMap playlist;
    QVariantMap snippet;
    QVariantMap status;
    snippet["title"] = "My playlist";
    snippet["description"] = "Playlist inserted using QYouTube";
    status["privacyStatus"] = "public";
    playlist["snippet"] = snippet;
    playlist["status"] = status;
    request.insert(playlist, "/playlists", QStringList() << "snippet" << "status");
    \endcode
*/
void ResourcesRequest::insert(const QVariantMap &resource, const QString &resourcePath, const QStringList &part,
                              const QVariantMap &params) {
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("part", part.join(","));
    
    if (!params.isEmpty()) {
        addUrlQueryItems(&query, params);
    }
    
    u.setQuery(query);
#else
    u.addQueryItem("part", part.join(","));
    
    if (!params.isEmpty()) {
        addUrlQueryItems(&u, params);
    }
#endif
    setUrl(u);
    setData(resource);
    post();
}

/*!
    \brief Updates the YouTube resource at \a resourcePath.
    
    For example, to update an existing playlist on behalf of the authenticated user:
    
    \code
    ResourcesRequest request;
    QVariantMap playlist;
    QVariantMap snippet;
    snippet["name"] = "My new playlist name";
    snippet["description"] = "My new playlist description";
    playlist["snippet"] = snippet;
    playlist["id"] = PLAYLIST_ID;
    request.update("/playlists", playlist, QStringList() << "snippet");
    \endcode
*/
void ResourcesRequest::update(const QString &resourcePath, const QVariantMap &resource, const QStringList &part) {
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("part", part.join(","));
    u.setQuery(query);
#else
    u.addQueryItem("part", part.join(","));
#endif
    setUrl(u);
    setData(resource);
    put();
}

/*!
    \brief Deletes the YouTube resource at \a resourcePath.
    
    For example, to remove a video from the authenticated user's favourites:
    
    \code
    ResourcesRequest request;
    request.del(FAVOURITE_PLAYLIST_ITEM_ID, "/playlistItems");
    \endcode
*/
void ResourcesRequest::del(const QString &id, const QString &resourcePath) {
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("id", id);
    u.setQuery(query);
#else
    u.addQueryItem("id", id);
#endif
    setUrl(u);
    deleteResource();
}

}