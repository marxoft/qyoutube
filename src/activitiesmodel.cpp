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

#include "activitiesmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class ActivitiesModelPrivate : public ModelPrivate
{

public:
    ActivitiesModelPrivate(ActivitiesModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ActivitiesModel);
    
        if (request->status() == ActivitiesRequest::Ready) {
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
        
        ActivitiesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onInsertRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ActivitiesModel);
    
        if (request->status() == ActivitiesRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                q->beginInsertRows(QModelIndex(), 0, 0);
                items.prepend(result);
                q->endInsertRows();
                emit q->countChanged();
            }
        }
        
        ActivitiesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onInsertRequestFinished()));
    
        emit q->statusChanged();
    }
    
    ActivitiesRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(ActivitiesModel)
};

/*!
    \class ActivitiesModel
    \brief A list model for displaying YouTube activity resources.
    
    \ingroup activities
    \ingroup models
    
    The ActivitiesModel is a list model used for displaying YouTube activities in a list view. ActivitiesModel provides 
    the same methods that are available in ActivitiesRequest, so  it is better to simply use that class if you do not 
    need the additional features provided by a data model.
    
    ActivitiesModel provides the following roles and role names:
    
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
            <td>ContentDetailsRole</td>
            <td>contentDetails</td>
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
    ActivitiesModel *model = new ActivitiesModel(this);
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    
    QVariantMap filters;
    filters["channelId"] = CHANNEL_ID;
    model->list(QStringList() << "snippet", filters);
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QYouTube 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: ActivitiesModel {
            id: activitiesModel
            
            apiKey: MY_API_KEY
        }
        delegate: Text {
            width: view.width
            height: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: snippet.title
        }
        
        Component.onCompleted: activitiesModel.list(["snippet"], {channelId: CHANNEL_ID})
    }
    \endcode
    
    \sa ActivitiesRequest
*/

/*!
    \enum ActivitiesModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/

ActivitiesModel::ActivitiesModel(QObject *parent) :
    Model(*new ActivitiesModelPrivate(this), parent)
{
    Q_D(ActivitiesModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new ActivitiesRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString ActivitiesModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa ActivitiesRequest::apiKey
*/

/*!
    \fn void ActivitiesModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString ActivitiesModel::apiKey() const {
    Q_D(const ActivitiesModel);
    
    return d->request->apiKey();
}

void ActivitiesModel::setApiKey(const QString &key) {
    Q_D(ActivitiesModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString ActivitiesModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa ActivitiesRequest::clientId
*/

/*!
    \fn void ActivitiesModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString ActivitiesModel::clientId() const {
    Q_D(const ActivitiesModel);
    
    return d->request->clientId();
}

void ActivitiesModel::setClientId(const QString &id) {
    Q_D(ActivitiesModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString ActivitiesModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa ActivitiesRequest::clientSecret
*/

/*!
    \fn void ActivitiesModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString ActivitiesModel::clientSecret() const {
    Q_D(const ActivitiesModel);
    
    return d->request->clientSecret();
}

void ActivitiesModel::setClientSecret(const QString &secret) {
    Q_D(ActivitiesModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString ActivitiesModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \as ActivitiesRequest::accessToken
*/

/*!
    \fn void ActivitiesModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString ActivitiesModel::accessToken() const {
    Q_D(const ActivitiesModel);
    
    return d->request->accessToken();
}

void ActivitiesModel::setAccessToken(const QString &token) {
    Q_D(ActivitiesModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString ActivitiesModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the accessToken needs to be refreshed.
    
    \sa ActivitiesRequest::refreshToken
*/

/*!
    \fn void ActivitiesModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString ActivitiesModel::refreshToken() const {
    Q_D(const ActivitiesModel);
    
    return d->request->refreshToken();
}

void ActivitiesModel::setRefreshToken(const QString &token) {
    Q_D(ActivitiesModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum ActivitiesModel::status
    \brief The current status of the model.
    
    \sa ActivitiesRequest::status
*/

/*!
    \fn void ActivitiesModel::statusChanged()
    \brief Emitted when the status changes.
*/
ActivitiesRequest::Status ActivitiesModel::status() const {
    Q_D(const ActivitiesModel);
    
    return d->request->status();
}

/*!
    \property enum ActivitiesModel::error
    \brief The error type of the model.
    
    \sa ActivitiesRequest::error
*/
ActivitiesRequest::Error ActivitiesModel::error() const {
    Q_D(const ActivitiesModel);
    
    return d->request->error();
}

/*!
    \property enum ActivitiesModel::errorString
    \brief A description of the error of the model.
    
    \sa ActivitiesRequest::status
*/
QString ActivitiesModel::errorString() const {
    Q_D(const ActivitiesModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    ActivitiesModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa ActivitiesRequest::setNetworkAccessManager()
*/
void ActivitiesModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(ActivitiesModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool ActivitiesModel::canFetchMore(const QModelIndex &) const {
    if (status() == ActivitiesRequest::Loading) {
        return false;
    }
    
    Q_D(const ActivitiesModel);
    
    return !d->nextPageToken.isEmpty();
}

void ActivitiesModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(ActivitiesModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube activity resources.
    
    \sa ActivitiesRequest::list()
*/
void ActivitiesModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != ActivitiesRequest::Loading) {
        Q_D(ActivitiesModel);
        d->part = part;
        d->filters = filters;
        d->params = params;
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(part, filters, params);
        emit statusChanged();
    }
}

/*!
    \brief Inserts a new YouTube activity resource.
    
    \sa ActivitiesRequest::insert()
*/
void ActivitiesModel::insert(const QVariantMap &resource, const QStringList &part) {
    if (status() != ActivitiesRequest::Loading) {
        Q_D(ActivitiesModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(resource, part);
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa ActivitiesRequest::cancel()
*/
void ActivitiesModel::cancel() {
    Q_D(ActivitiesModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube activity resources 
    using the existing parameters.
*/
void ActivitiesModel::reload() {
    if (status() != ActivitiesRequest::Loading) {
        Q_D(ActivitiesModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_activitiesmodel.cpp"
