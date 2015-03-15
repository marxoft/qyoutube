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

#include "webview.h"
#include "urls.h"
#include <QSettings>
#include <QMessageBox>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif
#ifdef QYOUTUBE_DEBUG
#include <QDebug>
#endif

WebView::WebView(QWidget *parent) :
    QWebView(parent)
{
    QSettings settings;
    request.setClientId(settings.value("Authentication/clientId").toString());
    request.setClientSecret(settings.value("Authentication/clientSecret").toString());
    request.setApiKey(settings.value("Authentication/apiKey").toString());
    request.setScopes(settings.value("Authentication/scopes").toStringList());
    
    connect(this, SIGNAL(titleChanged(QString)), this, SLOT(onTitleChanged(QString)));
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
        
    QUrl u(QYouTube::AUTH_URL);
#if QT_VERSION >= 0x050000
    QUrlQuery query(u);
    query.addQueryItem("client_id", request.clientId());
    query.addQueryItem("redirect_uri", QYouTube::REDIRECT_URI);
    query.addQueryItem("response_type", "code");
    query.addQueryItem("scope", request.scopes().join(" "));
    query.addQueryItem("access_type", "offline");
    query.addQueryItem("display", "popup");
    u.setQuery(query);
#else
    u.addQueryItem("client_id", request.clientId());
    u.addQueryItem("redirect_uri", QYouTube::REDIRECT_URI);
    u.addQueryItem("response_type", "code");
    u.addQueryItem("scope", request.scopes().join(" "));
    u.addQueryItem("access_type", "offline");
    u.addQueryItem("display", "popup");
#endif
#ifdef QYOUTUBE_DEBUG
    qDebug() << "WebView::setUrl" << u;
#endif
    setWindowTitle(tr("YouTube authentication"));
    setUrl(u);
}

void WebView::onTitleChanged(const QString &t) {
#ifdef QYOUTUBE_DEBUG
    qDebug() << "WebView::onTitleChanged" << t;
#endif
    if (t.contains("code=")) {
        request.exchangeCodeForAccessToken(t.section("code=", 1, 1).section('&', 0, 0));
    }
}

void WebView::onRequestFinished() {
    if (request.status() == QYouTube::AuthenticationRequest::Ready) {
        QVariantMap result = request.result().toMap();
        QString message = tr("Access token: %1\n\nExpires in: %2\n\nRefresh token: %3\n\nToken type: %4")
                            .arg(result.value("access_token").toString())
                            .arg(result.value("expires_in").toString())
                            .arg(result.value("refresh_token").toString())
                            .arg(result.value("token_type").toString());
                                                                
        QMessageBox::information(this, tr("Authentication sucessful"), message);
    }
    else {
        QMessageBox::information(this, tr("Authentication failed"), request.errorString());
    }
}
