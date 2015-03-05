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

#include "channelsectionsrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QStringList>

namespace QYouTube {

/*!
    \class ChannelSectionsRequest
    \brief Handles requests for YouTube channelSection resources.
    
    \ingroup channelsections
    \ingroup requests
    
    The ChannelSectionsRequest class is used for making requests to the YouTube Data API that concern YouTube 
    channelSection resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    ChannelSectionsRequest request;
    QVariantMap filters;
    filters["mine"] = true;
    request.setApiKey(MY_API_KEY);
    request.setClientId(MY_CLIENT_ID);
    request.setClientSecret(MY_CLIENT_SECRET);
    request.setAccessToken(MY_ACCESS_TOKEN);
    request.setRefreshToken(MY_REFRESH_TOKEN);
    request.list(QStringList() << "snippet", filters);
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == ChannelSectionsRequest::Ready) {            
            foreach (QVariant activity, request.result().toMap().value("items").toList()) {
                qDebug() << activity.toMap().value("snippet").toMap().value("title").toString();
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
    
    ChannelSectionsRequest {
        id: request
        
        apiKey: MY_API_KEY
        clientId: MY_CLIENT_ID
        clientSecret: MY_CLIENT_SECRET
        accessToken: MY_ACCESS_TOKEN
        refreshToken: MY_REFRESH_TOKEN
        onFinished: {
            if (status == ChannelSectionsRequest.Ready) {
                for (var i = 0; i < result.items.length; i++) {
                    console.log(result.items[i].snippet.title);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: list(["snippet"], {mine: true})
    }
    \endcode
    
    For more details about YouTube activities, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/channelSections">here</a>.
*/
ChannelSectionsRequest::ChannelSectionsRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a list of YouTube channelSection resources.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/channelSections/list">here</a>.
*/
void ChannelSectionsRequest::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {    
    QUrl u(CHANNEL_SECTIONS_URL);
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
    \brief Inserts a new YouTube channelSection resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/channelSections/insert">here</a>.
*/
void ChannelSectionsRequest::insert(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {    
    QUrl u(CHANNEL_SECTIONS_URL);
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
    \brief Updates an existing YouTube channelSection resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/channelSections/update">here</a>.
*/
void ChannelSectionsRequest::update(const QVariantMap &resource, const QStringList &part) {    
    QUrl u(CHANNEL_SECTIONS_URL);
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
    \brief Deletes an existing YouTube channelSection resource on behalf of the authenticated user.
    
    A valid access token is required for this operation.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/channelSections/delete">here</a>.
*/
void ChannelSectionsRequest::del(const QString &id) {    
    QUrl u(CHANNEL_SECTIONS_URL);
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
