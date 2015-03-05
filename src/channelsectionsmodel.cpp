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

#include "channelsectionsmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class ChannelSectionsModelPrivate : public ModelPrivate
{

public:
    ChannelSectionsModelPrivate(ChannelSectionsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ChannelSectionsModel);
    
        if (request->status() == ChannelSectionsRequest::Ready) {
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
        
        ChannelSectionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onInsertRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ChannelSectionsModel);
    
        if (request->status() == ChannelSectionsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                q->beginInsertRows(QModelIndex(), 0, 0);
                items.prepend(result);
                q->endInsertRows();
                emit q->countChanged();
            }
        }
        
        ChannelSectionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onInsertRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onUpdateRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ChannelSectionsModel);
    
        if (request->status() == ChannelSectionsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), ChannelSectionsModel::IdRole, id, 1, Qt::MatchExactly);
                    
                    if (!indexes.isEmpty()) {
                        q->set(indexes.first().row(), result);
                    }
                }
            }
        }
        
        ChannelSectionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onUpdateRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onDelRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ChannelSectionsModel);
    
        if (request->status() == ChannelSectionsRequest::Ready) {
            if (!delId.isEmpty()) {
                QModelIndexList indexes = q->match(QModelIndex(), ChannelSectionsModel::IdRole, delId, 1, Qt::MatchExactly);
                
                if (!indexes.isEmpty()) {
                    QModelIndex index = indexes.first();
                    q->beginRemoveRows(QModelIndex(), index.row(), index.row());
                    items.removeAt(index.row());
                    q->endRemoveRows();
                    emit q->countChanged();
                }
            }
        }
        
        ChannelSectionsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onDelRequestFinished()));
    
        emit q->statusChanged();
    }
        
    ChannelSectionsRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
    
    QString delId;
        
    QString previousPageToken;
    QString nextPageToken;
        
    Q_DECLARE_PUBLIC(ChannelSectionsModel)
};

/*!
    \class ChannelSectionsModel
    \brief A list model for displaying YouTube channelSection resources.
    
    \ingroup channelsections
    \ingroup models
    
    The ChannelSectionsModel is a list model used for displaying YouTube channelSection resources in a list view. 
    ChannelSectionsModel provides the same methods that are available in ChannelSectionsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    ChannelSectionsModel provides the following roles and role names:
    
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
    ChannelSectionsModel *model = new ChannelSectionsModel(this);
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
        model: ChannelSectionsModel {
            id: sectionsModel
            
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
        
        Component.onCompleted: sectionsModel.list(["snippet"], {channelId: CHANNEL_ID})
    }
    \endcode
    
    \sa ChannelSectionsRequest
*/

/*!
    \enum ChannelSectionsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
ChannelSectionsModel::ChannelSectionsModel(QObject *parent) :
    Model(*new ChannelSectionsModelPrivate(this), parent)
{
    Q_D(ChannelSectionsModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new ChannelSectionsRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString ChannelSectionsModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa ChannelSectionsRequest::apiKey
*/

/*!
    \fn void ChannelSectionsModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString ChannelSectionsModel::apiKey() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->apiKey();
}

void ChannelSectionsModel::setApiKey(const QString &key) {
    Q_D(ChannelSectionsModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString ChannelSectionsModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa ChannelSectionsRequest::clientId
*/

/*!
    \fn void ChannelSectionsModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString ChannelSectionsModel::clientId() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->clientId();
}

void ChannelSectionsModel::setClientId(const QString &id) {
    Q_D(ChannelSectionsModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString ChannelSectionsModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa ChannelSectionsRequest::clientSecret
*/

/*!
    \fn void ChannelSectionsModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString ChannelSectionsModel::clientSecret() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->clientSecret();
}

void ChannelSectionsModel::setClientSecret(const QString &secret) {
    Q_D(ChannelSectionsModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString ChannelSectionsModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa ChannelSectionsRequest::accessToken
*/

/*!
    \fn void ChannelSectionsModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString ChannelSectionsModel::accessToken() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->accessToken();
}

void ChannelSectionsModel::setAccessToken(const QString &token) {
    Q_D(ChannelSectionsModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString ChannelSectionsModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa ChannelSectionsRequest::refreshToken
*/

/*!
    \fn void ChannelSectionsModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString ChannelSectionsModel::refreshToken() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->refreshToken();
}

void ChannelSectionsModel::setRefreshToken(const QString &token) {
    Q_D(ChannelSectionsModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum ChannelSectionsModel::status
    \brief The current status of the model.
    
    \sa ChannelSectionsRequest::status
*/

/*!
    \fn void ChannelSectionsModel::statusChanged()
    \brief Emitted when the status changes.
*/
ChannelSectionsRequest::Status ChannelSectionsModel::status() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->status();
}

/*!
    \property enum ChannelSectionsModel::error
    \brief The error type of the model.
    
    \sa ChannelSectionsRequest::error
*/
ChannelSectionsRequest::Error ChannelSectionsModel::error() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->error();
}

/*!
    \property enum ChannelSectionsModel::errorString
    \brief A description of the error of the model.
    
    \sa ChannelSectionsRequest::status
*/
QString ChannelSectionsModel::errorString() const {
    Q_D(const ChannelSectionsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    ChannelSectionsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa ChannelSectionsRequest::setNetworkAccessManager()
*/
void ChannelSectionsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(ChannelSectionsModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool ChannelSectionsModel::canFetchMore(const QModelIndex &) const {
    if (status() == ChannelSectionsRequest::Loading) {
        return false;
    }
    
    Q_D(const ChannelSectionsModel);
    
    return !d->nextPageToken.isEmpty();
}

void ChannelSectionsModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(ChannelSectionsModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube channelSection resources.
    
    \sa ChannelSectionsRequest::list()
*/
void ChannelSectionsModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != ChannelSectionsRequest::Loading) {
        Q_D(ChannelSectionsModel);
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
    \brief Inserts a new YouTube channelSection resource.
    
    \sa ChannelSectionsRequest::insert()
*/
void ChannelSectionsModel::insert(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {
    if (status() != ChannelSectionsRequest::Loading) {
        Q_D(ChannelSectionsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(resource, part, params);
        emit statusChanged();
    }
}

/*!
    \brief Updates the specified YouTube channelSection resource.
    
    \sa ChannelSectionsRequest::update()
*/
void ChannelSectionsModel::update(int row, QVariantMap resource, const QStringList &part) {
    if (status() != ChannelSectionsRequest::Loading) {
        Q_D(ChannelSectionsModel);
        resource["id"] = data(index(row), IdRole);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->update(resource, part);
        emit statusChanged();
    }
}

/*!
    \brief Deletes the specified YouTube channelSection resource.
    
    \sa ChannelSectionsRequest::del()
*/
void ChannelSectionsModel::del(int row) {
    if (status() != ChannelSectionsRequest::Loading) {
        Q_D(ChannelSectionsModel);
        d->delId = data(index(row), IdRole).toString();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDelRequestFinished()));
        d->request->del(d->delId);
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa ChannelSectionsRequest::cancel()
*/
void ChannelSectionsModel::cancel() {
    Q_D(ChannelSectionsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube channelSection resources using the 
    existing parameters.
*/
void ChannelSectionsModel::reload() {
    if (status() != ChannelSectionsRequest::Loading) {
        Q_D(ChannelSectionsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_channelsectionsmodel.cpp"
