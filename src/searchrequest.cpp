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

#include "searchrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QStringList>

namespace QYouTube {

/*!
    \class SearchRequest
    \brief Handles requests for searching YouTube video, channel, and playlist resources.
    
    \ingroup search
    \ingroup requests
    
    The SearchRequest class is used for making requests to the YouTube Data API that concern YouTube video, 
    channel, and playlist resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    SearchRequest request;
    QVariantMap params;
    params["type"] = "video";
    params["order"] = "date";
    params["q"] = "Qt";
    request.setApiKey(MY_API_KEY);
    request.list(QStringList() << "snippet", QVariantMap(), params);
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == SearchRequest::Ready) {            
            foreach (QVariant video, request.result().toMap().value("items").toList()) {
                qDebug() << video.toMap().value("snippet").toMap().value("title").toString();
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
    
    SearchRequest {
        id: request
        
        apiKey: MY_API_KEY
        onFinished: {
            if (status == SearchRequest.Ready) {
                for (var i = 0; i < result.items.length; i++) {
                    console.log(result.items[i].snippet.title);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: list(["snippet"], {}, {type: "video", order: "date", q: "Qt"})
    }
    \endcode
    
    For more details about YouTube activities, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/search">here</a>.
*/
SearchRequest::SearchRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a search of YouTube video, channel, and playlist resources.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/search/list">here</a>.
*/
void SearchRequest::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {    
    QUrl u(SEARCH_URL);
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

}
