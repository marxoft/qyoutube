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

#include "streamsrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScriptEngine>
#include <QRegExp>
#include <QStringList>

namespace QYouTube {

class Format : public QVariantMap
{

public:
    Format() :
        QVariantMap()
    {
    }
    
    Format(const QString &id, const QString &desc, const QString &ext, int width, int height) :
        QVariantMap()
    {
        insert("id", id);
        insert("description", desc);
        insert("ext", ext);
        insert("width", width);
        insert("height", height);
    }
};

class FormatHash : public QHash<QString, Format>
{

public:
    FormatHash() : 
        QHash<QString, Format>() 
    {
        insert("5", Format("5", "FLV audio/video", "flv", 400, 240));
        insert("6", Format("6", "FLV audio/video", "flv", 450, 270));
        insert("17", Format("17", "3GP audio/video", "3gp", 176, 144));
        insert("18", Format("18", "MP4 audio/video", "mp4", 640, 360));
        insert("22", Format("22", "MP4 audio/video", "mp4", 1280, 720));
        insert("34", Format("34", "FLV audio/video", "flv", 640, 360));
        insert("35", Format("35", "FLV audio/video", "flv", 854, 480));
        insert("36", Format("36", "3GP audio/video", "3gp", 320, 240));
        insert("37", Format("37", "MP4 audio/video", "mp4", 1920, 1080));
        insert("38", Format("38", "MP4 audio/video", "mp4", 4096, 3072));
        insert("43", Format("43", "WebM audio/video", "webm", 640, 360));
        insert("44", Format("44", "WebM audio/video", "webm", 854, 480));
        insert("45", Format("45", "WebM audio/video", "webm", 1280, 720));
        insert("46", Format("46", "WebM audio/video", "webm", 1920, 1080));
        insert("82", Format("82", "MP4 3D audio/video", "mp4", 640, 360));
        insert("83", Format("83", "MP4 3D audio/video", "mp4", 854, 480));
        insert("84", Format("84", "MP4 3D audio/video", "mp4", 1280, 720));
        insert("85", Format("85", "MP4 3D audio/video", "mp4", 1920, 1080));
        insert("100", Format("100", "WebM 3D audio/video", "webm", 640, 360));
        insert("101", Format("101", "WebM 3D audio/video", "webm", 854, 480));
        insert("102", Format("102", "WebM 3D audio/video", "webm", 1280, 720));
        insert("92", Format("92", "MP4 HLS audio/video", "mp4", 400, 240));
        insert("93", Format("93", "MP4 HLS audio/video", "mp4", 640, 360));
        insert("94", Format("94", "MP4 HLS audio/video", "mp4", 854, 480));
        insert("95", Format("95", "MP4 HLS audio/video", "mp4", 1280, 720));
        insert("96", Format("96", "MP4 HLS audio/video", "mp4", 1920, 1080));
        insert("132", Format("132", "MP4 HLS audio/video", "mp4", 400, 240));
        insert("151", Format("92", "MP4 HLS audio/video", "mp4", 88, 72));
        insert("133", Format("133", "DASH MP4 video", "mp4", 400, 240));
        insert("134", Format("134", "DASH MP4 video", "mp4", 640, 360));
        insert("135", Format("135", "DASH MP4 video", "mp4", 854, 480));
        insert("136", Format("136", "DASH MP4 video", "mp4", 1280, 720));
        insert("137", Format("137", "DASH MP4 video", "mp4", 1920, 1080));
        insert("160", Format("160", "DASH MP4 video", "mp4", 176, 144));
        insert("264", Format("264", "DASH MP4 video", "mp4", 2560, 1440));
        insert("298", Format("298", "DASH MP4 video", "mp4", 1280, 720));
        insert("299", Format("299", "DASH MP4 video", "mp4", 1920, 1080));
        insert("266", Format("266", "DASH MP4 video", "mp4", 3840, 2160));
        insert("139", Format("139", "DASH MP4 audio", "m4a", 0, 0));
        insert("140", Format("140", "DASH MP4 audio", "m4a", 0, 0));
        insert("141", Format("141", "DASH MP4 audio", "m4a", 0, 0));
        insert("167", Format("167", "DASH WebM video", "webm", 640, 360));
        insert("168", Format("168", "DASH WebM video", "webm", 854, 480));
        insert("169", Format("169", "DASH WebM video", "webm", 1280, 720));
        insert("170", Format("170", "DASH WebM video", "webm", 1920, 1080));
        insert("218", Format("218", "DASH WebM video", "webm", 854, 480));
        insert("219", Format("219", "DASH WebM video", "webm", 854, 480));
        insert("278", Format("278", "DASH WebM video", "webm", 176, 144));
        insert("242", Format("242", "DASH WebM video", "webm", 400, 240));
        insert("243", Format("243", "DASH WebM video", "webm", 640, 360));
        insert("244", Format("244", "DASH WebM video", "webm", 854, 480));
        insert("245", Format("245", "DASH WebM video", "webm", 854, 480));
        insert("246", Format("246", "DASH WebM video", "webm", 854, 480));
        insert("247", Format("247", "DASH WebM video", "webm", 1280, 720));
        insert("248", Format("248", "DASH WebM video", "webm", 1920, 1080));
        insert("271", Format("271", "DASH WebM video", "webm", 2560, 1440));
        insert("272", Format("272", "DASH WebM video", "webm", 3840, 2160));
        insert("302", Format("302", "DASH WebM video", "webm", 1280, 720));
        insert("303", Format("303", "DASH WebM video", "webm", 1920, 1080));
        insert("308", Format("308", "DASH WebM video", "webm", 2560, 1440));
        insert("313", Format("313", "DASH WebM video", "webm", 3840, 2160));
        insert("315", Format("315", "DASH WebM video", "webm", 3840, 2160));
        insert("171", Format("171", "DASH WebM audio", "webm", 0, 0));
        insert("172", Format("172", "DASH WebM audio", "webm", 0, 0));
        insert("249", Format("249", "DASH WebM audio", "webm", 0, 0));
        insert("250", Format("250", "DASH WebM audio", "webm", 0, 0));
        insert("251", Format("251", "DASH WebM audio", "webm", 0, 0));
    }
};

class StreamsRequestPrivate : public RequestPrivate
{

static QString unescape(const QString &s) {
    int unescapes = 0;
    QByteArray us = s.toUtf8();

    while ((us.contains('%')) && (unescapes < 10)) {
        us = QByteArray::fromPercentEncoding(us);
        unescapes++;
    }
    
    return QString(us);
}

public:
    StreamsRequestPrivate(StreamsRequest *parent) :
        RequestPrivate(parent)
    {
        if (!decryptionEngine) {
            decryptionEngine = new QScriptEngine;
        }
    }
    
    void getVideoInfo() {
        Q_Q(StreamsRequest);
        
        QUrl u(VIDEO_INFO_URL);
#if QT_VERSION >= 0x050000
        QUrlQuery query;
        query.addQueryItem("video_id", id);
        query.addQueryItem("el", "detailpage");
        query.addQueryItem("ps", "default");
        query.addQueryItem("eurl", "gl");
        query.addQueryItem("gl", "US");
        query.addQueryItem("hl", "en");
        u.setQuery(query);
#else
        u.addQueryItem("video_id", id);
        u.addQueryItem("el", "detailpage");
        u.addQueryItem("ps", "default");
        u.addQueryItem("eurl", "gl");
        u.addQueryItem("gl", "US");
        u.addQueryItem("hl", "en");
#endif
#ifdef QYOUTUBE_DEBUG
        qDebug() << "QYouTube::StreamsRequestPrivate::getVideoInfo: Fetching video info" << u;
#endif
        if (reply) {
            delete reply;
        }
        
        q->setUrl(u);
        setOperation(StreamsRequest::GetOperation);
        setStatus(StreamsRequest::Loading);
        reply = networkAccessManager()->get(buildRequest(false));
        StreamsRequest::connect(reply, SIGNAL(finished()), q, SLOT(_q_onVideoInfoLoaded()));
    }
    
    void getVideoWebPage() {
        Q_Q(StreamsRequest);
        
        QUrl u(VIDEO_PAGE_URL);
#if QT_VERSION >= 0x050000
        QUrlQuery query;
        query.addQueryItem("v", id);
        query.addQueryItem("gl", "US");
        query.addQueryItem("hl", "en");
        query.addQueryItem("has_verified", "1");
        u.setQuery(query);
#else
        u.addQueryItem("v", id);
        u.addQueryItem("gl", "US");
        u.addQueryItem("hl", "en");
        u.addQueryItem("has_verified", "1");
#endif
#ifdef QYOUTUBE_DEBUG
        qDebug() << "QYouTube::StreamsRequestPrivate::getVideoWebPage: Fetching video web page" << u;
#endif
        if (reply) {
            delete reply;
        }
        
        q->setUrl(u);
        QNetworkRequest request = buildRequest(false);
        request.setRawHeader("User-Agent", "Wget/1.13.4 (linux-gnu)");
        setOperation(StreamsRequest::GetOperation);
        setStatus(StreamsRequest::Loading);
        reply = networkAccessManager()->get(request);
        StreamsRequest::connect(reply, SIGNAL(finished()), q, SLOT(_q_onVideoWebPageLoaded()));
    }
    
    QScriptValue getDecryptionFunction(const QUrl &playerUrl) {
        if (decryptionCache.contains(playerUrl)) {
            return decryptionCache.value(playerUrl);
        }
#ifdef QYOUTUBE_DEBUG
        qDebug() << "QYouTube::StreamsRequestPrivate::getDecryptionFunction: Fetching player JS" << playerUrl;
#endif
        Q_Q(StreamsRequest);
        
        if (reply) {
            delete reply;
        }
        
        q->setUrl(playerUrl);
        setOperation(StreamsRequest::GetOperation);
        setStatus(StreamsRequest::Loading);
        reply = networkAccessManager()->get(buildRequest(false));
        StreamsRequest::connect(reply, SIGNAL(finished()), q, SLOT(_q_onPlayerJSLoaded()));
        
        return QScriptValue();
    }
    
    void extractVideoStreams() {
        Q_Q(StreamsRequest);
        
        QVariantList formats;
        QStringList parts = response.split(',', QString::SkipEmptyParts);
        
        foreach (QString part, parts) {
            part = unescape(part);
            part.replace(QRegExp("(^|&)sig="), "&signature=");
            QStringList splitPart = part.split("url=");

            if (!splitPart.isEmpty()) {
                QString urlString = splitPart.last();
                QStringList params = urlString.mid(urlString.indexOf('?') + 1).split('&', QString::SkipEmptyParts);
                params.removeDuplicates();

                QUrl url(urlString.left(urlString.indexOf('?')));
#if QT_VERSION >= 0x050000
                QUrlQuery query;

                foreach (QString param, params) {
                    query.addQueryItem(param.section('=', 0, 0), param.section('=', -1));
                }

                if (!query.hasQueryItem("signature")) {
                    query.addQueryItem("signature", splitPart.first().section("signature=", 1, 1).section('&', 0, 0));
                }

                url.setQuery(query);
                
                Format format = formatHash.value(query.queryItemValue("itag"));
                format["url"] = url;
#else
                foreach (QString param, params) {
                    url.addQueryItem(param.section('=', 0, 0), param.section('=', -1));
                }

                if (!url.hasQueryItem("signature")) {
                    url.addQueryItem("signature", splitPart.first().section("signature=", 1, 1).section('&', 0, 0));
                }

                Format format = formatHash.value(url.queryItemValue("itag"));
                format["url"] = url;
#endif
                formats << format;
            }
        }
        
        setResult(formats);
        setStatus(StreamsRequest::Ready);
        setError(StreamsRequest::NoError);
        setErrorString(QString());
        emit q->finished();
    }
    
    void extractVideoStreams(QScriptValue decryptionFunction) {
        Q_Q(StreamsRequest);
        
        QVariantList formats;
        QStringList parts = response.split(',', QString::SkipEmptyParts);
        
        foreach (QString part, parts) {
            part = unescape(part);
            part.replace(QRegExp("(^|&)s="), "&signature=");
            QString oldSig = part.section("signature=", 1, 1).section('&', 0, 0);
            part.replace(oldSig, decryptionFunction.call(QScriptValue(), QScriptValueList() << oldSig).toString());
            QStringList splitPart = part.split("url=");

            if (!splitPart.isEmpty()) {
                QString urlString = splitPart.last();
                QStringList params = urlString.mid(urlString.indexOf('?') + 1).split('&', QString::SkipEmptyParts);
                params.removeDuplicates();

                QUrl url(urlString.left(urlString.indexOf('?')));
#if QT_VERSION >= 0x050000
                QUrlQuery query;

                foreach (QString param, params) {
                    query.addQueryItem(param.section('=', 0, 0), param.section('=', -1));
                }

                if (!query.hasQueryItem("signature")) {
                    query.addQueryItem("signature", splitPart.first().section("signature=", 1, 1).section('&', 0, 0));
                }

                url.setQuery(query);

                Format format = formatHash.value(query.queryItemValue("itag"));
                format["url"] = url;
#else
                foreach (QString param, params) {
                    url.addQueryItem(param.section('=', 0, 0), param.section('=', -1));
                }

                if (!url.hasQueryItem("signature")) {
                    url.addQueryItem("signature", splitPart.first().section("signature=", 1, 1).section('&', 0, 0));
                }

                Format format = formatHash.value(url.queryItemValue("itag"));
                format["url"] = url;
#endif
                formats << format;
            }
        }
        
        setResult(formats);
        setStatus(StreamsRequest::Ready);
        setError(StreamsRequest::NoError);
        setErrorString(QString());
        emit q->finished();
    }
    
    void _q_onVideoInfoLoaded() {
        if (!reply) {
            return;
        }
        
        Q_Q(StreamsRequest);
        
        switch (reply->error()) {
        case QNetworkReply::NoError:
            break;
        case QNetworkReply::OperationCanceledError:
            setStatus(StreamsRequest::Canceled);
            setError(StreamsRequest::NoError);
            setErrorString(QString());
            emit q->finished();
            return;
        default:
            setStatus(StreamsRequest::Failed);
            setError(StreamsRequest::Error(reply->error()));
            setErrorString(reply->errorString());
            emit q->finished();
            return;
        }
        
        response = reply->readAll();
        
        if (!response.contains("url_encoded_fmt_stream_map=")) {
#ifdef QYOUTUBE_DEBUG
            qDebug() << "QYouTube::StreamsRequestPrivate::_q_onVideoInfoLoaded: No format map in video info page. \
            Retrieving the web page";
#endif
            getVideoWebPage();
        }
        else {
            response = response.section("url_encoded_fmt_stream_map=", 1, 1);
            QString separator = response.left(response.indexOf('%'));

            if ((separator == "s") || (response.contains("%26s%3D"))) {
#ifdef QYOUTUBE_DEBUG
                qDebug() << "QYouTube::StreamsRequestPrivate::_q_onVideoInfoLoaded: Video has encrypted signatures. \
                Retrieving the web page";
#endif
                getVideoWebPage();
            }
            else {
#ifdef QYOUTUBE_DEBUG
                qDebug() << "QYouTube::StreamsRequestPrivate::_q_onVideoInfoLoaded: video info OK. Parsing the page";
#endif
                response = response.section('&', 0, 0).replace("%2C", ",");
                extractVideoStreams(response);
            }
        }
    }
    
    void _q_onVideoWebPageLoaded() {
        if (!reply) {
            return;
        }
        
        Q_Q(StreamsRequest);
        
        switch (reply->error()) {
        case QNetworkReply::NoError:
            break;
        case QNetworkReply::OperationCanceledError:
            setStatus(StreamsRequest::Canceled);
            setError(StreamsRequest::NoError);
            setErrorString(QString());
            emit q->finished();
            return;
        default:
            setStatus(StreamsRequest::Failed);
            setError(StreamsRequest::Error(reply->error()));
            setErrorString(reply->errorString());
            emit q->finished();
            return;
        }
        
        response = reply->readAll();
        
        if (response.contains("url_encoded_fmt_stream_map\":")) {
            QString js = response.section("\"assets\":", 1, 1).section('}', 0, 0) + "}";
            response = response.section("url_encoded_fmt_stream_map\":\"", 1, 1).section(",\"", 0, 0)
                                                                                .trimmed().replace("\\u0026", "&")
                                                                                .remove(QRegExp("itag=\\d+"));
        
            if (response.contains("sig=")) {
                extractVideoStreams();
            }
            else {        
                bool ok;
                QVariant assets = QtJson::Json::parse(js, ok);
                
                if (ok) {
                    QUrl playerUrl = assets.toMap().value("js").toUrl();

                    if (playerUrl.scheme().isEmpty()) {
                        playerUrl.setScheme("http");
                    }

                    if (playerUrl.isValid()) {
                        QScriptValue decryptionFunction = getDecryptionFunction(playerUrl);
                        
                        if (decryptionFunction.isFunction()) {
                            extractVideoStreams(decryptionFunction);
                        }
                        
                        return;
                    }
                }
            }
        }
            
        setStatus(StreamsRequest::Failed);
        setError(StreamsRequest::ParseError);
        setErrorString(StreamsRequest::tr("No video streams found for %1").arg(id));
        emit q->finished();
    }
    
    void _q_onPlayerJSLoaded() {
        if (!reply) {
            return;
        }
        
        Q_Q(StreamsRequest);
        
        switch (reply->error()) {
        case QNetworkReply::NoError:
            break;
        case QNetworkReply::OperationCanceledError:
            setStatus(StreamsRequest::Canceled);
            setError(StreamsRequest::NoError);
            setErrorString(QString());
            emit q->finished();
            return;
        default:
            setStatus(StreamsRequest::Failed);
            setError(StreamsRequest::Error(reply->error()));
            setErrorString(reply->errorString());
            emit q->finished();
            return;
        }
        
        QString jsresponse(reply->readAll());
        QRegExp re("\\.sig\\|\\|[\\w\\$]+(?=\\()");
        
        if (re.indexIn(jsresponse) != -1) {
            QString funcName = re.cap().section("||", -1);
            QString var = jsresponse.section("function " + funcName, 0, 0).section(";var", -1);
            QString funcBody = QString("function %2%3").arg(funcName).arg(jsresponse.section("function " + funcName, 1, 1)
                                                                                    .section(";function", 0, 0));
            QString js = QString("var%1 %2").arg(var).arg(funcBody);
#ifdef QYOUTUBE_DEBUG
            qDebug() << "QYouTube::StreamsRequestPrivate::_q_onPlayerJSLoaded: Found decryption function " << js;
#endif
            decryptionEngine->evaluate(js);

            QScriptValue global = decryptionEngine->globalObject();
            QScriptValue decryptionFunction = global.property(funcName);

            if (decryptionFunction.isFunction()) {
                decryptionCache[reply->request().url()] = decryptionFunction;
                extractVideoStreams(decryptionFunction);
                return;
            }
        }
        
        setStatus(StreamsRequest::Failed);
        setError(StreamsRequest::ParseError);
        setErrorString(StreamsRequest::tr("No video streams found for %1").arg(id));
        emit q->finished();
    }
    
    static QScriptEngine *decryptionEngine;
    
    static QHash<QUrl, QScriptValue> decryptionCache;
    
    static FormatHash formatHash;
        
    QString id;
    
    QString response;
    
    Q_DECLARE_PUBLIC(StreamsRequest)
};

QScriptEngine* StreamsRequestPrivate::decryptionEngine = 0;
QHash<QUrl, QScriptValue> StreamsRequestPrivate::decryptionCache;
FormatHash StreamsRequestPrivate::formatHash;

/*!
    \class StreamsRequest
    \brief Handles requests for video streams
    
    \ingroup streams
    \ingroup requests
    
    The StreamsRequest class is used for requesting a list of streams for a YouTube video.
    
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    StreamsRequest request;
    request.list(VIDEO_ID);
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == StreamsRequest::Ready) {
            foreach (QVariant stream, request.result().toList()) {
                qDebug() << "ID:" << stream.value("id").toString();
                qDebug() << "Description:" << stream.value("description").toString();
                qDebug() << "Extension:" << stream.value("ext").toString();
                qDebug() << "Width:" << stream.value("width").toInt();
                qDebug() << "Height:" << stream.value("height").toInt();
                qDebug() << "URL:" << stream.value("url").toString();
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
    
    StreamsRequest {
        id: request
        
        onFinished: {
            if (status == StreamsRequest.Ready) {
                for (var i = 0; i < result.length; i++) {
                    console.log("ID: " + result[i].id);
                    console.log("Description: " + result[i].description);
                    console.log("Extension: " + result[i].ext);
                    console.log("Width: " + result[i].width);
                    console.log("Height: " + result[i].height);
                    console.log("URL: " + result[i].url);
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
StreamsRequest::StreamsRequest(QObject *parent) :
    Request(*new StreamsRequestPrivate(this), parent)
{
}

/*!
    \brief Requests a list of streams for the video identified by id.
*/
void StreamsRequest::list(const QString &id) {
    Q_D(StreamsRequest);
    
    d->id = id;
    d->getVideoInfo();
}

}

#include "moc_streamsrequest.cpp"
