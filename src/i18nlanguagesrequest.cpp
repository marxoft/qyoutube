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

#include "i18nlanguagesrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QStringList>

namespace QYouTube {

/*!
    \class I18nLanguagesRequest
    \brief Handles requests for YouTube i18nLanguage resources.
    
    \ingroup 18nlanguages
    \ingroup requests
    
    The I18nLanguagesRequest class is used for making requests 
    to the YouTube Data API that concern YouTube i18nLanguage resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    I18nLanguagesRequest request;
    request.setApiKey(MY_API_KEY);
    request.list(QStringList() << "snippet");
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == I18nLanguagesRequest::Ready) {            
            foreach (QVariant activity, request.result().toMap().value("items").toList()) {
                qDebug() << activity.toMap().value("snippet").toMap().value("name").toString();
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
    
    I18nLanguagesRequest {
        id: request
        
        apiKey: MY_API_KEY
        onFinished: {
            if (status == I18nLanguagesRequest.Ready) {
                for (var i = 0; i < result.items.length; i++) {
                    console.log(result.items[i].snippet.name);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: list(["snippet"])
    }
    \endcode
    
    For more details about YouTube activities, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/i18nLanguages">here</a>.
*/
I18nLanguagesRequest::I18nLanguagesRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests a list of YouTube i18nLanguage resources.
    
    For more details of the acceptable parameter values, see the YouTube reference documentation 
    <a target="_blank" href="https://developers.google.com/youtube/v3/docs/i18nLanguages/list">here</a>.
*/ 
void I18nLanguagesRequest::list(const QStringList &part, const QVariantMap &params) {    
    QUrl u(I18N_LANGUAGES_URL);
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
    get();
}

}
