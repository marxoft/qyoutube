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

#ifndef QYOUTUBE_REQUEST_P_H
#define QYOUTUBE_REQUEST_P_H

#include "request.h"
#include "json.h"
#include <QUrl>
#include <QVariantMap>
#include <QNetworkRequest>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif
#ifdef QYOUTUBE_DEBUG
#include <QDebug>
#endif

class QNetworkReply;

namespace QYouTube {

static const int MAX_REDIRECTS = 8;

#if QT_VERSION >= 0x050000
inline void addUrlQueryItems(QUrlQuery *query, const QVariantMap &map) {
#ifdef QYOUTUBE_DEBUG
    qDebug() << "addUrlQueryItems:" << query << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            query->addQueryItem(iterator.key(), iterator.value().toString());
            break;
        default:
            query->addQueryItem(iterator.key(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}
#else
inline void addUrlQueryItems(QUrl *url, const QVariantMap &map) {
#ifdef QYOUTUBE_DEBUG
    qDebug() << "addUrlQueryItems:" << url << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            url->addQueryItem(iterator.key(), iterator.value().toString());
            break;
        default:
            url->addQueryItem(iterator.key(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}
#endif

inline void addRequestHeaders(QNetworkRequest *request, const QVariantMap &map) {
#ifdef QYOUTUBE_DEBUG
    qDebug() << "addRequestHeaders:" << request->url() << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            request->setRawHeader(iterator.key().toUtf8(), iterator.value().toByteArray());
            break;
        default:
            request->setRawHeader(iterator.key().toUtf8(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}

inline void addPostBody(QString *body, const QVariantMap &map) {
#ifdef QYOUTUBE_DEBUG
    qDebug() << "addPostBody:" << body << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            body->append(iterator.key() + "=" + iterator.value().toString());
            break;
        default:
            body->append(iterator.key() + "=" + QtJson::Json::serialize(iterator.value()));
            break;
        }
        
        if (iterator.hasNext()) {
            body->append("&");
        }
    }
}

class RequestPrivate
{

public:
    RequestPrivate(Request *parent);
    virtual ~RequestPrivate();
    
    QNetworkAccessManager* networkAccessManager();
    
    void setOperation(Request::Operation op);
    
    void setStatus(Request::Status s);
    
    void setError(Request::Error e);
    
    void setErrorString(const QString &es);
    
    void setResult(const QVariant &res);
    
    virtual QNetworkRequest buildRequest(bool authRequired = true);
    virtual QNetworkRequest buildRequest(QUrl u, bool authRequired = true);
    
    virtual void followRedirect(const QUrl &redirect);
        
    void refreshAccessToken();
    virtual void _q_onAccessTokenRefreshed();
    
    virtual void _q_onReplyFinished();
    
    Request *q_ptr;
    
    QNetworkAccessManager *manager;
    
    QNetworkReply *reply;
    
    bool ownNetworkAccessManager;
    
    QString apiKey;
    QString clientId;
    QString clientSecret;
    QString accessToken;
    QString refreshToken;
        
    QUrl url;
    
    QVariantMap headers;
    
    QVariant data;
    
    QVariant result;
    
    Request::Operation operation;
    
    Request::Status status;
    
    Request::Error error;
    
    QString errorString;
    
    int redirects;
    
    Q_DECLARE_PUBLIC(Request)
};

}

#endif // QYOUTUBE_REQUEST_P_H
