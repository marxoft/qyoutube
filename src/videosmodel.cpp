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

#include "videosmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class VideosModelPrivate : public ModelPrivate
{

public:
    VideosModelPrivate(VideosModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(VideosModel);
    
        if (request->status() == VideosRequest::Ready) {
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
        
        VideosModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onUpdateRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(VideosModel);
    
        if (request->status() == VideosRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), VideosModel::IdRole, id, 1, Qt::MatchExactly);
                    
                    if (!indexes.isEmpty()) {
                        q->set(indexes.first().row(), result);
                    }
                }
            }
        }
        
        VideosModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onUpdateRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onDelRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(VideosModel);
    
        if (request->status() == VideosRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), VideosModel::IdRole, id, 1, Qt::MatchExactly);
                    
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
        
        VideosModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onDelRequestFinished()));
    
        emit q->statusChanged();
    }
        
    VideosRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(VideosModel)
};

/*!
    \class VideosModel
    \brief A list model for displaying YouTube video resources.
    
    \ingroup videos
    \ingroup models
    
    The VideosModel is a list model used for displaying YouTube video resources in a list view. 
    VideosModel provides the same methods that are available in VideosRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    VideosModel provides the following roles and role names:
    
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
            <td>AuditDetailsRole</td>
            <td>auditDetails</td>
        </tr>
        <tr>
            <td>BrandingSettingsRole</td>
            <td>brandingSettings</td>
        </tr>
        <tr>
            <td>ContentDetailsRole</td>
            <td>contentDetails</td>
        </tr>
        <tr>
            <td>FileDetailsRole</td>
            <td>fileDetails</td>
        </tr>
        <tr>
            <td>IdRole</td>
            <td>id</td>
        </tr>
        <tr>
            <td>LiveStreamingDetailsRole</td>
            <td>liveStreamingDetails</td>
        </tr>
        <tr>
            <td>PlayerRole</td>
            <td>player</td>
        </tr>
        <tr>
            <td>ProcessingDetailsRole</td>
            <td>processingDetails</td>
        </tr>
        <tr>
            <td>RecordingDetailsRole</td>
            <td>recordingDetails</td>
        </tr>
        <tr>
            <td>SnippetRole</td>
            <td>snippet</td>
        </tr>
        <tr>
            <td>StatisticsRole</td>
            <td>statistics</td>
        </tr>
        <tr>
            <td>StatusRole</td>
            <td>status</td>
        </tr>
        <tr>
            <td>SuggestionsRole</td>
            <td>suggestions</td>
        </tr>
        <tr>
            <td>TopicDetailsRole</td>
            <td>topicDetails</td>
        </tr>
    </table>
        
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    QListView *view = new QListView(this);
    VideosModel *model = new VideosModel(this);
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    
    QVariantMap filters;
    filters["chart"] = "mostPopular";
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
        model: VideosModel {
            id: videosModel
            
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
        
        Component.onCompleted: videosModel.list(["snippet"], {chart: "mostPopular"})
    }
    \endcode
    
    \sa VideosRequest
*/

/*!
    \enum VideosModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
VideosModel::VideosModel(QObject *parent) :
    Model(*new VideosModelPrivate(this), parent)
{
    Q_D(VideosModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[FileDetailsRole] = "fileDetails";
    d->roles[IdRole] = "id";
    d->roles[LiveStreamingDetailsRole] = "liveStreamingDetails";
    d->roles[PlayerRole] = "player";
    d->roles[ProcessingDetailsRole] = "processingDetails";
    d->roles[RecordingDetailsRole] = "recordingDetails";
    d->roles[SnippetRole] = "snippet";
    d->roles[StatisticsRole] = "statistics";
    d->roles[StatusRole] = "status";
    d->roles[SuggestionsRole] = "suggestions";
    d->roles[TopicDetailsRole] = "topicDetails";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new VideosRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString VideosModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa VideosRequest::apiKey
*/

/*!
    \fn void VideosModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString VideosModel::apiKey() const {
    Q_D(const VideosModel);
    
    return d->request->apiKey();
}

void VideosModel::setApiKey(const QString &key) {
    Q_D(VideosModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString VideosModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa VideosRequest::clientId
*/

/*!
    \fn void VideosModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString VideosModel::clientId() const {
    Q_D(const VideosModel);
    
    return d->request->clientId();
}

void VideosModel::setClientId(const QString &id) {
    Q_D(VideosModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString VideosModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa VideosRequest::clientSecret
*/

/*!
    \fn void VideosModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString VideosModel::clientSecret() const {
    Q_D(const VideosModel);
    
    return d->request->clientSecret();
}

void VideosModel::setClientSecret(const QString &secret) {
    Q_D(VideosModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString VideosModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa VideosRequest::accessToken
*/

/*!
    \fn void VideosModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString VideosModel::accessToken() const {
    Q_D(const VideosModel);
    
    return d->request->accessToken();
}

void VideosModel::setAccessToken(const QString &token) {
    Q_D(VideosModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString VideosModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa VideosRequest::refreshToken
*/

/*!
    \fn void VideosModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString VideosModel::refreshToken() const {
    Q_D(const VideosModel);
    
    return d->request->refreshToken();
}

void VideosModel::setRefreshToken(const QString &token) {
    Q_D(VideosModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum VideosModel::status
    \brief The current status of the model.
    
    \sa VideosRequest::status
*/

/*!
    \fn void VideosModel::statusChanged()
    \brief Emitted when the status changes.
*/
VideosRequest::Status VideosModel::status() const {
    Q_D(const VideosModel);
    
    return d->request->status();
}

/*!
    \property enum VideosModel::error
    \brief The error type of the model.
    
    \sa VideosRequest::error
*/
VideosRequest::Error VideosModel::error() const {
    Q_D(const VideosModel);
    
    return d->request->error();
}

/*!
    \property enum VideosModel::errorString
    \brief A description of the error of the model.
    
    \sa VideosRequest::status
*/
QString VideosModel::errorString() const {
    Q_D(const VideosModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    VideosModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa VideosRequest::setNetworkAccessManager()
*/
void VideosModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(VideosModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool VideosModel::canFetchMore(const QModelIndex &) const {
    if (status() == VideosRequest::Loading) {
        return false;
    }
    
    Q_D(const VideosModel);
    
    return !d->nextPageToken.isEmpty();
}

void VideosModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(VideosModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube video resources.
    
    \sa VideosRequest::list()
*/
void VideosModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != VideosRequest::Loading) {
        Q_D(VideosModel);
        d->part = part;
        d->filters = filters;
        d->params = params;
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(part, filters, params);
        emit statusChanged();
    }
}

/*!
    \brief Updates the specified YouTube video resource.
    
    \sa VideosRequest::update()
*/
void VideosModel::update(int row, QVariantMap resource, const QStringList &part, const QVariantMap &params) {
    if (status() != VideosRequest::Loading) {
        Q_D(VideosModel);
        resource["id"] = data(index(row), IdRole);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->update(resource, part, params);
        emit statusChanged();
    }
}

/*!
    \brief Sets the specified YouTube video resource's rating.
    
    \sa VideosRequest::rate()
*/
void VideosModel::rate(int row, const QString &rating) {
    if (status() != VideosRequest::Loading) {
        Q_D(VideosModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->rate(data(index(row), IdRole).toString(), rating);
        emit statusChanged();
    }
}

/*!
    \brief Deletes the specified YouTube video resource.
    
    \sa VideosRequest::del()
*/
void VideosModel::del(int row) {
    if (status() != VideosRequest::Loading) {
        Q_D(VideosModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDelRequestFinished()));
        d->request->del(data(index(row), IdRole).toString());
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa VideosRequest::cancel()
*/
void VideosModel::cancel() {
    Q_D(VideosModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube video resources using the 
    existing parameters.
*/
void VideosModel::reload() {
    if (status() != VideosRequest::Loading) {
        Q_D(VideosModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_videosmodel.cpp"
