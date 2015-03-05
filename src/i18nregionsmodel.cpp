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

#include "i18nregionsmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class I18nRegionsModelPrivate : public ModelPrivate
{

public:
    I18nRegionsModelPrivate(I18nRegionsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(I18nRegionsModel);
    
        if (request->status() == I18nRegionsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                previousPageToken = result.value("prevPageToken").toString();
                nextPageToken = result.value("nextPageToken").toString();
            
                QVariantList list = result.value("items").toList();
            
                if (!list.isEmpty()) {
                    q->beginInsertRows(QModelIndex(), items.size(), items.size() + list.size());
                    
                    foreach (QVariant item, list) {
                        items << item.toMap();
                    }
                    
                    q->endInsertRows();
                    emit q->countChanged();
                }
            }
        }
        
        I18nRegionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    I18nRegionsRequest *request;
    
    QStringList part;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(I18nRegionsModel)
};

/*!
    \class I18nRegionsModel
    \brief A list model for displaying YouTube i18nRegion resources.
    
    \ingroup i18nregions
    \ingroup models
    
    The I18nRegionsModel is a list model used for displaying YouTube i18nRegion resources in a list view. 
    I18nRegionsModel provides the same methods that are available in I18nRegionsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    I18nRegionsModel provides the following roles and role names:
    
    <table>
        <tr>
        <th>Role</th>
        <th>Role name</th>
        </tr>
        <tr>
            <td>KindRole</td>
            <td>kind</td>
        </tr>
        <tr>
            <td>EtagRole</td>
            <td>etag</td>
        </tr>
        <tr>
            <td>IdRole</td>
            <td>id</td>
        </tr>
        <tr>
            <td>SnippetRole</td>
            <td>snippet</td>
        </tr>
    </table>
        
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    QListView *view = new QListView(this);
    I18nRegionsModel *model = new I18nRegionsModel(this);
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    model->list(QStringList() << "snippet");
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QYouTube 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: I18nRegionsModel {
            id: regionsModel
            
            apiKey: MY_API_KEY
        }
        delegate: Text {
            width: view.width
            height: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: snippet.name
        }
        
        Component.onCompleted: regionsModel.list(["snippet"])
    }
    \endcode
    
    \sa I18nRegionsRequest
*/

/*!
    \enum I18nRegionsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
I18nRegionsModel::I18nRegionsModel(QObject *parent) :
    Model(*new I18nRegionsModelPrivate(this), parent)
{
    Q_D(I18nRegionsModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new I18nRegionsRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString I18nRegionsModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa I18nRegionsRequest::apiKey
*/

/*!
    \fn void I18nRegionsModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString I18nRegionsModel::apiKey() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->apiKey();
}

void I18nRegionsModel::setApiKey(const QString &key) {
    Q_D(I18nRegionsModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString I18nRegionsModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa I18nRegionsRequest::clientId
*/

/*!
    \fn void I18nRegionsModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString I18nRegionsModel::clientId() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->clientId();
}

void I18nRegionsModel::setClientId(const QString &id) {
    Q_D(I18nRegionsModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString I18nRegionsModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa I18nRegionsRequest::clientSecret
*/

/*!
    \fn void I18nRegionsModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString I18nRegionsModel::clientSecret() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->clientSecret();
}

void I18nRegionsModel::setClientSecret(const QString &secret) {
    Q_D(I18nRegionsModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString I18nRegionsModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa I18nRegionsRequest::accessToken
*/

/*!
    \fn void I18nRegionsModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString I18nRegionsModel::accessToken() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->accessToken();
}

void I18nRegionsModel::setAccessToken(const QString &token) {
    Q_D(I18nRegionsModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString I18nRegionsModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa I18nRegionsRequest::refreshToken
*/

/*!
    \fn void I18nRegionsModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString I18nRegionsModel::refreshToken() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->refreshToken();
}

void I18nRegionsModel::setRefreshToken(const QString &token) {
    Q_D(I18nRegionsModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum I18nRegionsModel::status
    \brief The current status of the model.
    
    \sa I18nRegionsRequest::status
*/

/*!
    \fn void I18nRegionsModel::statusChanged()
    \brief Emitted when the status changes.
*/
I18nRegionsRequest::Status I18nRegionsModel::status() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->status();
}

/*!
    \property enum I18nRegionsModel::error
    \brief The error type of the model.
    
    \sa I18nRegionsRequest::error
*/
I18nRegionsRequest::Error I18nRegionsModel::error() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->error();
}

/*!
    \property enum I18nRegionsModel::errorString
    \brief A description of the error of the model.
    
    \sa I18nRegionsRequest::status
*/
QString I18nRegionsModel::errorString() const {
    Q_D(const I18nRegionsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    I18nRegionsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa I18nRegionsRequest::setNetworkAccessManager()
*/
void I18nRegionsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(I18nRegionsModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool I18nRegionsModel::canFetchMore(const QModelIndex &) const {
    if (status() == I18nRegionsRequest::Loading) {
        return false;
    }
    
    Q_D(const I18nRegionsModel);
    
    return !d->nextPageToken.isEmpty();
}

void I18nRegionsModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(I18nRegionsModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, params);
    }
}

/*!
    \brief Retrieves a list of YouTube i18nRegion resources.
    
    \sa I18nRegionsRequest::list()
*/
void I18nRegionsModel::list(const QStringList &part, const QVariantMap &params) {
    if (status() != I18nRegionsRequest::Loading) {
        Q_D(I18nRegionsModel);
        d->part = part;
        d->params = params;
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(part, params);
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa I18nRegionsRequest::cancel()
*/
void I18nRegionsModel::cancel() {
    Q_D(I18nRegionsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube i18nRegion resources using the 
    existing parameters.
*/
void I18nRegionsModel::reload() {
    if (status() != I18nRegionsRequest::Loading) {
        Q_D(I18nRegionsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->params);
        emit statusChanged();
    }
}

}

#include "moc_i18nregionsmodel.cpp"
