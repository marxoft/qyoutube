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

#include "playlistitemsrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QStringList>

namespace QYouTube {

/*!
    \class PlaylistItemsRequest
    \brief Handles requests for YouTube playlistItem resources.
    
    \ingroup playlistitems
    \ingroup requests
    
    The PlaylistItemsRequest class is used for making requests 
    to the YouTube Data API that concern YouTube playlistItem resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;

    ...

    PlaylistItemsRequest request;
    QVariantMap item;
    QVariantMap snippet;
    snippet["playlistId"] = PLAYLIST_ID;
    snippet["resourceId"] = RESOURCE_ID;
    snippet["position"] = 1;
    item["snippet"] = snippet;
    request.setApiKey(MY_API_KEY);
    request.setClientId(MY_CLIENT_ID);
    request.setClientSecret(MY_CLIENT_SECRET);
    request.setAccessToken(MY_ACCESS_TOKEN);
    request.setRefreshToken(MY_REFRESH_TOKEN);
    request.insert(item, QStringList() << "snippet");
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == PlaylistItemsRequest::Ready) {            
            qDebug() << request.result().toMap().value("snippet").toMap().value("position").toInt();
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
    
    PlaylistItemsRequest {
        id: request
        
        apiKey: MY_API_KEY
        clientId: MY_CLIENT_ID
        clientSecret: MY_CLIENT_SECRET
        accessToken: MY_ACCESS_TOKEN
        refreshToken: MY_REFRESH_TOKEN
        onFinished: {
            if (status == PlaylistItemsRequest.Ready) {
                console.log(result.snippet.position);
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: insert({snippet: {playlistId: PLAYLIST_ID, resourceId: RESOURCE_ID, position: 1}}, 
        ["snippet"])
    }
    \endcode
    
    For more details about YouTube activities, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlistItems">here</a>.
*/
PlaylistItemsRequest::PlaylistItemsRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a list of YouTube playlistItem resources.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlistItems/list">here</a>.
*/
void PlaylistItemsRequest::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {    
    QUrl u(PLAYLIST_ITEMS_URL);
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
    \brief Inserts a new YouTube playlistItem resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlistItems/insert">here</a>.
*/
void PlaylistItemsRequest::insert(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {    
    QUrl u(PLAYLIST_ITEMS_URL);
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
    \brief Updates an existing YouTube playlistItem resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlistItems/insert">here</a>.
*/
void PlaylistItemsRequest::update(const QVariantMap &resource, const QStringList &part) {    
    QUrl u(PLAYLIST_ITEMS_URL);
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
    \brief Deletes an existing YouTube playlistItem resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlistItems/insert">here</a>.
*/
void PlaylistItemsRequest::del(const QString &id) {    
    QUrl u(PLAYLIST_ITEMS_URL);
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
