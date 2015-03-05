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

#include "playlistitemsmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class PlaylistItemsModelPrivate : public ModelPrivate
{

public:
    PlaylistItemsModelPrivate(PlaylistItemsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistItemsModel);
    
        if (request->status() == PlaylistItemsRequest::Ready) {
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
        
        PlaylistItemsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onInsertRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistItemsModel);
    
        if (request->status() == PlaylistItemsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                q->beginInsertRows(QModelIndex(), 0, 0);
                items.prepend(result);
                q->endInsertRows();
                emit q->countChanged();
            }
        }
        
        PlaylistItemsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onInsertRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onUpdateRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistItemsModel);
    
        if (request->status() == PlaylistItemsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), PlaylistItemsModel::IdRole, id, 1, Qt::MatchExactly);
                    
                    if (!indexes.isEmpty()) {
                        q->set(indexes.first().row(), result);
                    }
                }
            }
        }
        
        PlaylistItemsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onUpdateRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onDelRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistItemsModel);
    
        if (request->status() == PlaylistItemsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), PlaylistItemsModel::IdRole, id, 1, Qt::MatchExactly);
                    
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
        
        PlaylistItemsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onDelRequestFinished()));
    
        emit q->statusChanged();
    }
        
    PlaylistItemsRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(PlaylistItemsModel)
};

/*!
    \class PlaylistItemsModel
    \brief A list model for displaying YouTube playlistItem resources.
    
    \ingroup playlistitems
    \ingroup models
    
    The PlaylistItemsModel is a list model used for displaying YouTube playlistItem resources in a list view. 
    PlaylistItemsModel provides the same methods that are available in PlaylistItemsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    PlaylistItemsModel provides the following roles and role names:
    
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
            <td>StatusRole</td>
            <td>status</td>
        </tr>
    </table>
        
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    QListView *view = new QListView(this);
    PlaylistItemsModel *model = new PlaylistItemsModel(this);
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    
    QVariantMap filters;
    filters["playlistId"] = PLAYLIST_ID;
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
        model: PlaylistItemsModel {
            id: playlistItemsModel
            
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
        
        Component.onCompleted: playlistItemsModel.list(["snippet"], {playlistId: PLAYLIST_ID})
    }
    \endcode
    
    \sa PlaylistItemsRequest
*/

/*!
    \enum PlaylistItemsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
PlaylistItemsModel::PlaylistItemsModel(QObject *parent) :
    Model(*new PlaylistItemsModelPrivate(this), parent)
{
    Q_D(PlaylistItemsModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
    d->roles[StatusRole] = "status";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new PlaylistItemsRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString PlaylistItemsModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa PlaylistItemsRequest::apiKey
*/

/*!
    \fn void PlaylistItemsModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString PlaylistItemsModel::apiKey() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->apiKey();
}

void PlaylistItemsModel::setApiKey(const QString &key) {
    Q_D(PlaylistItemsModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString PlaylistItemsModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa PlaylistItemsRequest::clientId
*/

/*!
    \fn void PlaylistItemsModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString PlaylistItemsModel::clientId() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->clientId();
}

void PlaylistItemsModel::setClientId(const QString &id) {
    Q_D(PlaylistItemsModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString PlaylistItemsModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa PlaylistItemsRequest::clientSecret
*/

/*!
    \fn void PlaylistItemsModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString PlaylistItemsModel::clientSecret() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->clientSecret();
}

void PlaylistItemsModel::setClientSecret(const QString &secret) {
    Q_D(PlaylistItemsModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString PlaylistItemsModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa PlaylistItemsRequest::accessToken
*/

/*!
    \fn void PlaylistItemsModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString PlaylistItemsModel::accessToken() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->accessToken();
}

void PlaylistItemsModel::setAccessToken(const QString &token) {
    Q_D(PlaylistItemsModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString PlaylistItemsModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa PlaylistItemsRequest::refreshToken
*/

/*!
    \fn void PlaylistItemsModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString PlaylistItemsModel::refreshToken() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->refreshToken();
}

void PlaylistItemsModel::setRefreshToken(const QString &token) {
    Q_D(PlaylistItemsModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum PlaylistItemsModel::status
    \brief The current status of the model.
    
    \sa PlaylistItemsRequest::status
*/

/*!
    \fn void PlaylistItemsModel::statusChanged()
    \brief Emitted when the status changes.
*/
PlaylistItemsRequest::Status PlaylistItemsModel::status() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->status();
}

/*!
    \property enum PlaylistItemsModel::error
    \brief The error type of the model.
    
    \sa PlaylistItemsRequest::error
*/
PlaylistItemsRequest::Error PlaylistItemsModel::error() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->error();
}

/*!
    \property enum PlaylistItemsModel::errorString
    \brief A description of the error of the model.
    
    \sa PlaylistItemsRequest::status
*/
QString PlaylistItemsModel::errorString() const {
    Q_D(const PlaylistItemsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    PlaylistItemsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa PlaylistItemsRequest::setNetworkAccessManager()
*/
void PlaylistItemsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(PlaylistItemsModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool PlaylistItemsModel::canFetchMore(const QModelIndex &) const {
    if (status() == PlaylistItemsRequest::Loading) {
        return false;
    }
    
    Q_D(const PlaylistItemsModel);
    
    return !d->nextPageToken.isEmpty();
}

void PlaylistItemsModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(PlaylistItemsModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube playlistItem resources.
    
    \sa PlaylistItemsRequest::list()
*/
void PlaylistItemsModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != PlaylistItemsRequest::Loading) {
        Q_D(PlaylistItemsModel);
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
    \brief Inserts a new YouTube playlistItem resource.
    
    \sa PlaylistItemsRequest::update()
*/
void PlaylistItemsModel::insert(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {
    if (status() != PlaylistItemsRequest::Loading) {
        Q_D(PlaylistItemsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(resource, part, params);
        emit statusChanged();
    }
}

/*!
    \brief Updates the specified YouTube playlistItem resource.
    
    \sa PlaylistItemsRequest::update()
*/
void PlaylistItemsModel::update(int row, QVariantMap resource, const QStringList &part) {
    if (status() != PlaylistItemsRequest::Loading) {
        Q_D(PlaylistItemsModel);
        resource["id"] = data(index(row), IdRole);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->update(resource, part);
        emit statusChanged();
    }
}

/*!
    \brief Deletes the specified YouTube playlistItem resource.
    
    \sa PlaylistItemsRequest::del()
*/
void PlaylistItemsModel::del(int row) {
    if (status() != PlaylistItemsRequest::Loading) {
        Q_D(PlaylistItemsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDelRequestFinished()));
        d->request->del(data(index(row), IdRole).toString());
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa PlaylistItemsRequest::cancel()
*/
void PlaylistItemsModel::cancel() {
    Q_D(PlaylistItemsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube playlistItem resources using the 
    existing parameters.
*/
void PlaylistItemsModel::reload() {
    if (status() != PlaylistItemsRequest::Loading) {
        Q_D(PlaylistItemsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_playlistitemsmodel.cpp"
