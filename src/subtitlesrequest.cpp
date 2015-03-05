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

#include "subtitlesrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QDomDocument>
#include <QDomElement>
#include <QNetworkReply>

namespace QYouTube {

class SubtitlesRequestPrivate : public RequestPrivate
{

public:
    SubtitlesRequestPrivate(SubtitlesRequest *parent) :
        RequestPrivate(parent)
    {
    }
    
    void onReplyFinished() {
        if (!reply) {
            return;
        }
    
        Q_Q(SubtitlesRequest);
    
        if (redirects < MAX_REDIRECTS) {
            QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    
            if (redirect.isEmpty()) {
                redirect = reply->header(QNetworkRequest::LocationHeader).toUrl();
            }
    
            if (!redirect.isEmpty()) {
                followRedirect(redirect);
            }
        }
    
        switch (reply->error()) {
        case QNetworkReply::NoError:
            break;
        case QNetworkReply::OperationCanceledError:
            setStatus(Request::Canceled);
            setError(Request::NoError);
            setErrorString(QString());
            emit q->finished();
            return;
        default:
            setStatus(Request::Failed);
            setError(Request::Error(reply->error()));
            setErrorString(reply->errorString());
            emit q->finished();
            return;
        }
        
        QDomDocument doc;
        doc.setContent(reply->readAll());
        QDomElement transcriptEl = doc.documentElement();
        QDomNodeList trackNodes = transcriptEl.elementsByTagName("track");
        QVariantList subs;

        for (int i = 0; i < trackNodes.size(); i++) {
            QDomElement trackEl = trackNodes.at(i).toElement();
            QVariantMap sub;
            QUrl u(SUBTITLES_URL);
            QString code = trackEl.attribute("lang_code");
#if QT_VERSION >= 0x050000
            QUrlQuery query(u);
            query.addQueryItem("v", QUrlQuery(reply->request().url()).queryItemValue("v"));
            query.addQueryItem("lang", code);
            u.setQuery(query);
#else
            u.addQueryItem("v", reply->request().url().queryItemValue("v"));
            u.addQueryItem("lang", code);
#endif
            sub["language"] = trackEl.attribute("lang");
            sub["languageCode"] = code;
            sub["url"] = u;
            subs << sub;
        }
        
        setResult(subs);
        setStatus(Request::Ready);
        setError(Request::NoError);
        setErrorString(QString());
        emit q->finished();
    }
    
    Q_DECLARE_PUBLIC(SubtitlesRequest)
};

/*!
    \class SubtitlesRequest
    \brief Handles requests for video subtitles
    
    \ingroup subtitles
    \ingroup requests
    
    The SubtitlesRequest class is used for requesting a list of subtitles for a YouTube video.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    SubtitlesRequest request;
    request.list(VIDEO_ID);
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == SubtitlesRequest::Ready) {
            foreach (QVariant subs, request.result().toList()) {
                qDebug() << subs.value("language").toString();
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
    
    SubtitlesRequest {
        id: request
        
        onFinished: {
            if (status == SubtitlesRequest.Ready) {
                for (var i = 0; i < result.length; i++) {
                    console.log(result[i].language);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: list(VIDEO_ID)
    }
    \endcode
*/
SubtitlesRequest::SubtitlesRequest(QObject *parent) :
    Request(*new SubtitlesRequestPrivate(this), parent)
{
}

/*!
    \brief Requests a list of subtitles for the video identified by id.
*/
void SubtitlesRequest::list(const QString &id) {    
    QUrl u(SUBTITLES_URL);
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("v", id);
    u.setQuery(query);
#else
    u.addQueryItem("v", id);
#endif
    setUrl(u);
    get();
}

}
