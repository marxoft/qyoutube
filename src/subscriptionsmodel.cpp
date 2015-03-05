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

#include "subscriptionsmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class SubscriptionsModelPrivate : public ModelPrivate
{

public:
    SubscriptionsModelPrivate(SubscriptionsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(SubscriptionsModel);
    
        if (request->status() == SubscriptionsRequest::Ready) {
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
        
        SubscriptionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onInsertRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(SubscriptionsModel);
    
        if (request->status() == SubscriptionsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                q->beginInsertRows(QModelIndex(), 0, 0);
                items.prepend(result);
                q->endInsertRows();
                emit q->countChanged();
            }
        }
        
        SubscriptionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onInsertRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onDelRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(SubscriptionsModel);
    
        if (request->status() == SubscriptionsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), SubscriptionsModel::IdRole, id, 1, Qt::MatchExactly);
                    
                    if (!indexes.isEmpty()) {
                        QModelIndex index = indexes.first();
                        q->beginRemoveRows(QModelIndex(), index.row(), index.row());
                        items.removeAt(index.row());
                        q->endRemoveRows();
                        emit q->countChanged();
                    }
                }
            }
        }
        
        SubscriptionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onDelRequestFinished()));
    
        emit q->statusChanged();
    }
        
    SubscriptionsRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(SubscriptionsModel)
};

/*!
    \class SubscriptionsModel
    \brief A list model for displaying YouTube subscription resources.
    
    \ingroup subscriptions
    \ingroup models
    
    The SubscriptionsModel is a list model used for displaying YouTube subscription resources in a list view. 
    SubscriptionsModel provides the same methods that are available in SubscriptionsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    SubscriptionsModel provides the following roles and role names:
    
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
        <tr>
            <td>SubscriberSnippetRole</td>
            <td>subscriberSnippet</td>
        </tr>
    </table>
        
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    QListView *view = new QListView(this);
    SubscriptionsModel *model = new SubscriptionsModel(this);
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
        model: SubscriptionsModel {
            id: subscriptionsModel
            
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
        
        Component.onCompleted: subscriptionsModel.list(["snippet"], {channelId: CHANNEL_ID})
    }
    \endcode
    
    \sa SubscriptionsRequest
*/

/*!
    \enum SubscriptionsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
SubscriptionsModel::SubscriptionsModel(QObject *parent) :
    Model(*new SubscriptionsModelPrivate(this), parent)
{
    Q_D(SubscriptionsModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
    d->roles[SubscriberSnippetRole] = "subscriberSnippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new SubscriptionsRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString SubscriptionsModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa SubscriptionsRequest::apiKey
*/

/*!
    \fn void SubscriptionsModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString SubscriptionsModel::apiKey() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->apiKey();
}

void SubscriptionsModel::setApiKey(const QString &key) {
    Q_D(SubscriptionsModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString SubscriptionsModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa SubscriptionsRequest::clientId
*/

/*!
    \fn void SubscriptionsModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString SubscriptionsModel::clientId() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->clientId();
}

void SubscriptionsModel::setClientId(const QString &id) {
    Q_D(SubscriptionsModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString SubscriptionsModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa SubscriptionsRequest::clientSecret
*/

/*!
    \fn void SubscriptionsModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString SubscriptionsModel::clientSecret() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->clientSecret();
}

void SubscriptionsModel::setClientSecret(const QString &secret) {
    Q_D(SubscriptionsModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString SubscriptionsModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa SubscriptionsRequest::accessToken
*/

/*!
    \fn void SubscriptionsModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString SubscriptionsModel::accessToken() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->accessToken();
}

void SubscriptionsModel::setAccessToken(const QString &token) {
    Q_D(SubscriptionsModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString SubscriptionsModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa SubscriptionsRequest::refreshToken
*/

/*!
    \fn void SubscriptionsModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString SubscriptionsModel::refreshToken() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->refreshToken();
}

void SubscriptionsModel::setRefreshToken(const QString &token) {
    Q_D(SubscriptionsModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum SubscriptionsModel::status
    \brief The current status of the model.
    
    \sa SubscriptionsRequest::status
*/

/*!
    \fn void SubscriptionsModel::statusChanged()
    \brief Emitted when the status changes.
*/
SubscriptionsRequest::Status SubscriptionsModel::status() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->status();
}

/*!
    \property enum SubscriptionsModel::error
    \brief The error type of the model.
    
    \sa SubscriptionsRequest::error
*/
SubscriptionsRequest::Error SubscriptionsModel::error() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->error();
}

/*!
    \property enum SubscriptionsModel::errorString
    \brief A description of the error of the model.
    
    \sa SubscriptionsRequest::status
*/
QString SubscriptionsModel::errorString() const {
    Q_D(const SubscriptionsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    SubscriptionsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa SubscriptionsRequest::setNetworkAccessManager()
*/
void SubscriptionsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(SubscriptionsModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool SubscriptionsModel::canFetchMore(const QModelIndex &) const {
    if (status() == SubscriptionsRequest::Loading) {
        return false;
    }
    
    Q_D(const SubscriptionsModel);
    
    return !d->nextPageToken.isEmpty();
}

void SubscriptionsModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(SubscriptionsModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube subscription resources.
    
    \sa SubscriptionsRequest::list()
*/
void SubscriptionsModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != SubscriptionsRequest::Loading) {
        Q_D(SubscriptionsModel);
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
    \brief Inserts a new YouTube subscription resource.
    
    \sa SubscriptionsRequest::update()
*/
void SubscriptionsModel::insert(const QVariantMap &resource, const QStringList &part) {
    if (status() != SubscriptionsRequest::Loading) {
        Q_D(SubscriptionsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(resource, part);
        emit statusChanged();
    }
}

/*!
    \brief Deletes the specified YouTube subscription resource.
    
    \sa SubscriptionsRequest::del()
*/
void SubscriptionsModel::del(int row) {
    if (status() != SubscriptionsRequest::Loading) {
        Q_D(SubscriptionsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDelRequestFinished()));
        d->request->del(data(index(row), IdRole).toString());
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa SubscriptionsRequest::cancel()
*/
void SubscriptionsModel::cancel() {
    Q_D(SubscriptionsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube subscription resources using the 
    existing parameters.
*/
void SubscriptionsModel::reload() {
    if (status() != SubscriptionsRequest::Loading) {
        Q_D(SubscriptionsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_subscriptionsmodel.cpp"
