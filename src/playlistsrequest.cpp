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

#include "playlistsrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QStringList>

namespace QYouTube {

/*!
    \class PlaylistsRequest
    \brief Handles requests for YouTube playlist resources.
    
    \ingroup playlists
    \ingroup requests
    
    The PlaylistsRequest class is used for making requests to the YouTube Data API that concern YouTube playlist 
    resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    PlaylistsRequest request;
    QVariantMap playlist;
    QVariantMap snippet;
    snippet["title"] = "My playlist";
    snippet["description"] = "A playlist created using QYouTube";
    snippet["tags"] = QStringList() << "QYouTube" << "Qt";
    playlist["snippet"] = snippet;
    request.setApiKey(MY_API_KEY);
    request.setClientId(MY_CLIENT_ID);
    request.setClientSecret(MY_CLIENT_SECRET);
    request.setAccessToken(MY_ACCESS_TOKEN);
    request.setRefreshToken(MY_REFRESH_TOKEN);
    request.insert(item, QStringList() << "snippet");
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == PlaylistsRequest::Ready) {            
            qDebug() << request.result().toMap().value("snippet").toMap().value("title").toString();
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
    
    PlaylistsRequest {
        id: request
        
        apiKey: MY_API_KEY
        clientId: MY_CLIENT_ID
        clientSecret: MY_CLIENT_SECRET
        accessToken: MY_ACCESS_TOKEN
        refreshToken: MY_REFRESH_TOKEN
        onFinished: {
            if (status == PlaylistsRequest.Ready) {
                console.log(result.snippet.title);
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: insert({snippet: {title: "My playlist", description: "A playlist created using QYouTube", 
        tags: ["QYouTube", "Qt"]}}, ["snippet"])
    }
    \endcode
    
    For more details about YouTube activities, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlists">here</a>.
*/
PlaylistsRequest::PlaylistsRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a list of YouTube playlist resources.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlists/list">here</a>.
*/
void PlaylistsRequest::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {    
    QUrl u(PLAYLISTS_URL);
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
    \brief Inserts a new YouTube playlist resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlists/insert">here</a>.
*/
void PlaylistsRequest::insert(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {    
    QUrl u(PLAYLISTS_URL);
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
    \brief Updates an existing YouTube playlist resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlists/insert">here</a>.
*/
void PlaylistsRequest::update(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {    
    QUrl u(PLAYLISTS_URL);
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
    put();
}

/*!
    \brief Deletes an existing YouTube playlist resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/playlists/delete">here</a>.
*/
void PlaylistsRequest::del(const QString &id) {    
    QUrl u(PLAYLISTS_URL);
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
