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

#ifndef VIDEOCATEGORIESMODEL_H
#define VIDEOCATEGORIESMODEL_H

#include "model.h"
#include "videocategoriesrequest.h"

namespace QYouTube {

class VideoCategoriesModelPrivate;

class QYOUTUBESHARED_EXPORT VideoCategoriesModel : public Model
{
    Q_OBJECT
    
    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString clientSecret READ clientSecret WRITE setClientSecret NOTIFY clientSecretChanged)
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged)
    Q_PROPERTY(QYouTube::VideoCategoriesRequest::Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QYouTube::VideoCategoriesRequest::Error error READ error NOTIFY statusChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY statusChanged)
                
public:
    enum Roles {
        KindRole = Qt::UserRole + 1,
        EtagRole,
        IdRole,
        SnippetRole
    };
        
    explicit VideoCategoriesModel(QObject *parent = 0);
    
    QString apiKey() const;
    void setApiKey(const QString &key);
    
    QString clientId() const;
    void setClientId(const QString &id);
    
    QString clientSecret() const;
    void setClientSecret(const QString &secret);
    
    QString accessToken() const;
    void setAccessToken(const QString &token);
    
    QString refreshToken() const;
    void setRefreshToken(const QString &token);
    
    VideoCategoriesRequest::Status status() const;
    
    VideoCategoriesRequest::Error error() const;
    QString errorString() const;
    
    void setNetworkAccessManager(QNetworkAccessManager *manager);
    
    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
    void fetchMore(const QModelIndex &parent = QModelIndex());
    
public Q_SLOTS:
    void list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params = QVariantMap());
        
    void cancel();
    void reload();
    
Q_SIGNALS:
    void apiKeyChanged();
    void clientIdChanged();
    void clientSecretChanged();
    void accessTokenChanged();
    void refreshTokenChanged();
    void statusChanged();
    
private:        
    Q_DECLARE_PRIVATE(VideoCategoriesModel)
    Q_DISABLE_COPY(VideoCategoriesModel)
    
    Q_PRIVATE_SLOT(d_func(), void _q_onListRequestFinished())
};

}

#endif // VIDEOSCATEGORIESMODEL_H
