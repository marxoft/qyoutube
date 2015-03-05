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

#include "channelsmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class ChannelsModelPrivate : public ModelPrivate
{

public:
    ChannelsModelPrivate(ChannelsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ChannelsModel);
    
        if (request->status() == ChannelsRequest::Ready) {
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
        
        ChannelsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    void _q_onUpdateRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ChannelsModel);
    
        if (request->status() == ChannelsRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QString id = result.value("id").toString();
                
                if (!id.isEmpty()) {
                    QModelIndexList indexes = q->match(QModelIndex(), ChannelsModel::IdRole, id, 1, Qt::MatchExactly);
                    
                    if (!indexes.isEmpty()) {
                        q->set(indexes.first().row(), result);
                    }
                }
            }
        }
        
        ChannelsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onUpdateRequestFinished()));
    
        emit q->statusChanged();
    }
    
    ChannelsRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(ChannelsModel)
};

/*!
    \class ChannelsModel
    \brief A list model for displaying YouTube channel resources.
    
    \ingroup channels
    \ingroup models
    
    The ChannelsModel is a list model used for displaying YouTube channel resources in a list view. 
    ChannelsModel provides the same methods that are available in ChannelsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    ChannelsModel provides the following roles and role names:
    
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
            <td>ContentOwnerDetailsRole</td>
            <td>contentOwnerDetails</td>
        </tr>
        <tr>
            <td>IdRole</td>
            <td>id</td>
        </tr>
        <tr>
            <td>InVideoPromotionRole</td>
            <td>invideoPromotion</td>
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
    ChannelsModel *model = new ChannelsModel(this);
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
        model: ChannelsModel {
            id: sModel
            
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
        
        Component.onCompleted: sModel.list(["snippet"], {channelId: CHANNEL_ID})
    }
    \endcode
    
    \sa ChannelsRequest
*/

/*!
    \enum ChannelsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
ChannelsModel::ChannelsModel(QObject *parent) :
    Model(*new ChannelsModelPrivate(this), parent)
{
    Q_D(ChannelsModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[AuditDetailsRole] = "auditDetails";
    d->roles[BrandingSettingsRole] = "brandingSettings";
    d->roles[ContentDetailsRole] = "contentDetails";
    d->roles[ContentOwnerDetailsRole] = "contentOwnerDetails";
    d->roles[IdRole] = "id";
    d->roles[InVideoPromotionRole] = "invideoPromotion";
    d->roles[SnippetRole] = "snippet";
    d->roles[StatisticsRole] = "statistics";
    d->roles[StatusRole] = "status";
    d->roles[TopicDetailsRole] = "topicDetails";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new ChannelsRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString ChannelsModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa ChannelsRequest::apiKey
*/

/*!
    \fn void ChannelsModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString ChannelsModel::apiKey() const {
    Q_D(const ChannelsModel);
    
    return d->request->apiKey();
}

void ChannelsModel::setApiKey(const QString &key) {
    Q_D(ChannelsModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString ChannelsModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa ChannelsRequest::clientId
*/

/*!
    \fn void ChannelsModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString ChannelsModel::clientId() const {
    Q_D(const ChannelsModel);
    
    return d->request->clientId();
}

void ChannelsModel::setClientId(const QString &id) {
    Q_D(ChannelsModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString ChannelsModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa ChannelsRequest::clientSecret
*/

/*!
    \fn void ChannelsModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString ChannelsModel::clientSecret() const {
    Q_D(const ChannelsModel);
    
    return d->request->clientSecret();
}

void ChannelsModel::setClientSecret(const QString &secret) {
    Q_D(ChannelsModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString ChannelsModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa ChannelsRequest::accessToken
*/

/*!
    \fn void ChannelsModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString ChannelsModel::accessToken() const {
    Q_D(const ChannelsModel);
    
    return d->request->accessToken();
}

void ChannelsModel::setAccessToken(const QString &token) {
    Q_D(ChannelsModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString ChannelsModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa ChannelsRequest::refreshToken
*/

/*!
    \fn void ChannelsModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString ChannelsModel::refreshToken() const {
    Q_D(const ChannelsModel);
    
    return d->request->refreshToken();
}

void ChannelsModel::setRefreshToken(const QString &token) {
    Q_D(ChannelsModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum ChannelsModel::status
    \brief The current status of the model.
    
    \sa ChannelsRequest::status
*/

/*!
    \fn void ChannelsModel::statusChanged()
    \brief Emitted when the status changes.
*/
ChannelsRequest::Status ChannelsModel::status() const {
    Q_D(const ChannelsModel);
    
    return d->request->status();
}

/*!
    \property enum ChannelsModel::error
    \brief The error type of the model.
    
    \sa ChannelsRequest::error
*/
ChannelsRequest::Error ChannelsModel::error() const {
    Q_D(const ChannelsModel);
    
    return d->request->error();
}

/*!
    \property enum ChannelsModel::errorString
    \brief A description of the error of the model.
    
    \sa ChannelsRequest::status
*/
QString ChannelsModel::errorString() const {
    Q_D(const ChannelsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    ChannelsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa ChannelsRequest::setNetworkAccessManager()
*/
void ChannelsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(ChannelsModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool ChannelsModel::canFetchMore(const QModelIndex &) const {
    if (status() == ChannelsRequest::Loading) {
        return false;
    }
    
    Q_D(const ChannelsModel);
    
    return !d->nextPageToken.isEmpty();
}

void ChannelsModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(ChannelsModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube channel resources.
    
    \sa ChannelsRequest::list()
*/
void ChannelsModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != ChannelsRequest::Loading) {
        Q_D(ChannelsModel);
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
    \brief Updates the specified YouTube channel resource.
    
    \sa ChannelsRequest::update()
*/
void ChannelsModel::update(const QVariantMap &resource, const QStringList &part, const QVariantMap &params) {
    if (status() != ChannelsRequest::Loading) {
        Q_D(ChannelsModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->update(resource, part, params);
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa ChannelsRequest::cancel()
*/
void ChannelsModel::cancel() {
    Q_D(ChannelsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube channel resources using the 
    existing parameters.
*/
void ChannelsModel::reload() {
    if (status() != ChannelsRequest::Loading) {
        Q_D(ChannelsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_channelsmodel.cpp"
