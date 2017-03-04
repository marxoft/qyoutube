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
    
    void _q_onReplyFinished() {
        Q_Q(SubtitlesRequest);
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(q->sender());

        if (redirects < MAX_REDIRECTS) {
            QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    
            if (redirect.isEmpty()) {
                redirect = reply->header(QNetworkRequest::LocationHeader).toUrl();
            }
    
            if (!redirect.isEmpty()) {
                deleteReply(reply);
                followRedirect(redirect);
            }
        }
        
        const QByteArray response = reply->readAll();
        const QNetworkReply::NetworkError e = reply->error();
        const QString es = reply->errorString();
        const QUrl subtitlesUrl = reply->request().url();
        deleteReply(reply);
    
        switch (e) {
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
            setError(Request::Error(e));
            setErrorString(es);
            emit q->finished();
            return;
        }
        
        QDomDocument doc;
        doc.setContent(response);
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
            query.addQueryItem("v", QUrlQuery(subtitlesUrl).queryItemValue("v"));
            query.addQueryItem("lang", code);
            u.setQuery(query);
#else
            u.addQueryItem("v", subtitlesUrl.queryItemValue("v"));
            u.addQueryItem("lang", code);
#endif
            sub["id"] = trackEl.attribute("id");
            sub["originalLanguage"] = trackEl.attribute("lang_original");
            sub["translatedLanguage"] = trackEl.attribute("lang_translated");
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
    if (status() == Loading) {
        return;
    }
    
    QUrl u(SUBTITLES_URL);
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("hl", "en");
    query.addQueryItem("type", "list");
    query.addQueryItem("v", id);
    u.setQuery(query);
#else
    u.addQueryItem("hl", "en");
    u.addQueryItem("type", "list");
    u.addQueryItem("v", id);
#endif
    setUrl(u);
    get(false);
}

}
