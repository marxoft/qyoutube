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

#include "playlistsmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class PlaylistsModelPrivate : public ModelPrivate
{

public:
    PlaylistsModelPrivate(PlaylistsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistsModel);
    
        if (request->status() == PlaylistsRequest::Ready) {
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
        
        PlaylistsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onInsertRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistsModel);
    
        if (request->status() == PlaylistsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                q->beginInsertRows(QModelIndex(), 0, 0);
                items.prepend(result);
                q->endInsertRows();
                emit q->countChanged();
            }
        }
        
        PlaylistsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onInsertRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onUpdateRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistsModel);
    
        if (request->status() == PlaylistsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), PlaylistsModel::IdRole, id, 1, Qt::MatchExactly);
                    
                    if (!indexes.isEmpty()) {
                        q->set(indexes.first().row(), result);
                    }
                }
            }
        }
        
        PlaylistsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onUpdateRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onDelRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(PlaylistsModel);
    
        if (request->status() == PlaylistsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), PlaylistsModel::IdRole, id, 1, Qt::MatchExactly);
                    
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
        
        PlaylistsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onDelRequestFinished()));
    
        emit q->statusChanged();
    }
        
    PlaylistsRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(PlaylistsModel)
};

/*!
    \class PlaylistsModel
    \brief A list model for displaying YouTube playlist resources.
    
    \ingroup playlists
    \ingroup models
    
    The PlaylistsModel is a list model used for displaying YouTube playlist resources in a list view. 
    PlaylistsModel provides the same methods that are available in PlaylistsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    PlaylistsModel provides the following roles and role names:
    
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
            <td>PlayerRole</td>
            <td>player</td>
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
    PlaylistsModel *model = new PlaylistsModel(this);
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
        model: PlaylistsModel {
            id: playlistsModel
            
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
        
        Component.onCompleted: playlistsModel.list(["snippet"], {channelId: CHANNEL_ID})
    }
    \endcode
    
    \sa PlaylistsRequest
*/

/*!
    \enum PlaylistsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
PlaylistsModel::PlaylistsModel(QObject *parent) :
    Model(*new PlaylistsModelPrivate(this), parent)
{
    Q_D(PlaylistsModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[IdRole] = "id";
    d->roles[PlayerRole] = "player";
    d->roles[SnippetRole] = "snippet";
    d->roles[StatusRole] = "status";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new PlaylistsRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString PlaylistsModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa PlaylistsRequest::apiKey
*/

/*!
    \fn void PlaylistsModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString PlaylistsModel::apiKey() const {
    Q_D(const PlaylistsModel);
    
    return d->request->apiKey();
}

void PlaylistsModel::setApiKey(const QString &key) {
    Q_D(PlaylistsModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString PlaylistsModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa PlaylistsRequest::clientId
*/

/*!
    \fn void PlaylistsModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString PlaylistsModel::clientId() const {
    Q_D(const PlaylistsModel);
    
    return d->request->clientId();
}

void PlaylistsModel::setClientId(const QString &id) {
    Q_D(PlaylistsModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString PlaylistsModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa PlaylistsRequest::clientSecret
*/

/*!
    \fn void PlaylistsModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString PlaylistsModel::clientSecret() const {
    Q_D(const PlaylistsModel);
    
    return d->request->clientSecret();
}

void PlaylistsModel::setClientSecret(const QString &secret) {
    Q_D(PlaylistsModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString PlaylistsModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa PlaylistsRequest::accessToken
*/

/*!
    \fn void PlaylistsModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString PlaylistsModel::accessToken() const {
    Q_D(const PlaylistsModel);
    
    return d->request->accessToken();
}

void PlaylistsModel::setAccessToken(const QString &token) {
    Q_D(PlaylistsModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString PlaylistsModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa PlaylistsRequest::refreshToken
*/

/*!
    \fn void PlaylistsModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString PlaylistsModel::refreshToken() const {
    Q_D(const PlaylistsModel);
    
    return d->request->refreshToken();
}

void PlaylistsModel::setRefreshToken(const QString &token) {
    Q_D(PlaylistsModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum PlaylistsModel::status
    \brief The current status of the model.
    
    \sa PlaylistsRequest::status
*/

/*!
    \fn void PlaylistsModel::statusChanged()
    \brief Emitted when the status changes.
*/
PlaylistsRequest::Status PlaylistsModel::status() const {
    Q_D(const PlaylistsModel);
    
    return d->request->status();
}

/*!
    \property enum PlaylistsModel::error
    \brief The error type of the model.
    
    \sa PlaylistsRequest::error
*/
PlaylistsRequest::Error PlaylistsModel::error() const {
    Q_D(const PlaylistsModel);
    
    return d->request->error();
}

/*!
    \property enum PlaylistsModel::errorString
    \brief A description of the error of the model.
    
    \sa PlaylistsRequest::status
*/
QString PlaylistsModel::errorString() const {
    Q_D(const PlaylistsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    PlaylistsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa PlaylistsRequest::setNetworkAccessManager()
*/
void PlaylistsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(PlaylistsModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool PlaylistsModel::canFetchMore(const QModelIndex &) const {
    if (status() == PlaylistsRequest::Loading) {
        return false;
    }
    
    Q_D(const PlaylistsModel);
    
    return !d->nextPageToken.isEmpty();
}

void PlaylistsModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(PlaylistsModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube playlist resources.
    
    \sa PlaylistsRequest::list()
*/
void PlaylistsModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != PlaylistsRequest::Loading) {
        Q_D(PlaylistsModel);
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
    \brief Inserts a new YouTube playlist resource.
    
    \sa PlaylistsRequest::update()
*/
void PlaylistsModel::insert(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {
    if (status() != PlaylistsRequest::Loading) {
        Q_D(PlaylistsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(resource, part, params);
        emit statusChanged();
    }
}

/*!
    \brief Updates the specified YouTube playlist resource.
    
    \sa PlaylistsRequest::update()
*/
void PlaylistsModel::update(int row, QVariantMap resource, const QStringList &part, const QVariantMap &params) {
    if (status() != PlaylistsRequest::Loading) {
        Q_D(PlaylistsModel);
        resource["id"] = data(index(row), IdRole);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->update(resource, part, params);
        emit statusChanged();
    }
}

/*!
    \brief Deletes the specified YouTube playlist resource.
    
    \sa PlaylistsRequest::del()
*/
void PlaylistsModel::del(int row) {
    if (status() != PlaylistsRequest::Loading) {
        Q_D(PlaylistsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDelRequestFinished()));
        d->request->del(data(index(row), IdRole).toString());
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa PlaylistsRequest::cancel()
*/
void PlaylistsModel::cancel() {
    Q_D(PlaylistsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube playlist resources using the 
    existing parameters.
*/
void PlaylistsModel::reload() {
    if (status() != PlaylistsRequest::Loading) {
        Q_D(PlaylistsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_playlistsmodel.cpp"
