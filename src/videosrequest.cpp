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

#include "videosrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QStringList>

namespace QYouTube {

/*!
    \class VideosRequest
    \brief Handles requests for YouTube video resources.
    
    \ingroup videos
    \ingroup requests
    
    The VideosRequest class is used for making requests to the YouTube Data API that concern YouTube video resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    VideosRequest request;
    request.setApiKey(MY_API_KEY);
    request.setClientId(MY_CLIENT_ID);
    request.setClientSecret(MY_CLIENT_SECRET);
    request.setAccessToken(MY_ACCESS_TOKEN);
    request.setRefreshToken(MY_REFRESH_TOKEN);
    request.rate(VIDEO_ID, "like");
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == VideosRequest::Ready) {            
            qDebug() << "Video rated";
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
    
    VideosRequest {
        id: request
        
        apiKey: MY_API_KEY
        clientId: MY_CLIENT_ID
        clientSecret: MY_CLIENT_SECRET
        accessToken: MY_ACCESS_TOKEN
        refreshToken: MY_REFRESH_TOKEN
        onFinished: {
            if (status == VideosRequest.Ready) {
                console.log("Video rated");
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: rate(VIDEO_ID, "like")
    }
    \endcode
    
    For more details about YouTube activities, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/videos">here</a>.
*/
VideosRequest::VideosRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a list of YouTube video resources.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/videos/list">here</a>.
*/
void VideosRequest::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {    
    QUrl u(VIDEOS_URL);
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
    \brief Updates an existing YouTube video resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/videos/insert">here</a>.
*/
void VideosRequest::update(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {    
    QUrl u(VIDEOS_URL);
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
    \brief Rates an existing YouTube video resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/videos/rate">here</a>.
*/
void VideosRequest::rate(const QString &id, const QString &rating) {    
    QUrl u(VIDEOS_URL);
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("id", id);
    query.addQueryItem("rating", rating);
    u.setQuery(query);
#else
    u.addQueryItem("id", id);
    u.addQueryItem("rating", rating);
#endif
    setUrl(u);
    post();
}

/*!
    \brief Retrieves the rating for an existing YouTube video resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/videos/getRating">here</a>.
*/
void VideosRequest::getRating(const QString &id, const QVariantMap &params) {    
    QUrl u(VIDEOS_URL);
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("id", id);
    
    if (!params.isEmpty()) {
        addUrlQueryItems(&query, params);
    }
    
    u.setQuery(query);
#else
    u.addQueryItem("id", id);
    
    if (!params.isEmpty()) {
        addUrlQueryItems(&u, params);
    }
#endif
    setUrl(u);
    get();
}

/*!
    \brief Deletes an existing YouTube video resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/videos/delete">here</a>.
*/
void VideosRequest::del(const QString &id) {    
    QUrl u(VIDEOS_URL);
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
