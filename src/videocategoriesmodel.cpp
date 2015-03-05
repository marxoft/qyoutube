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

#include "videocategoriesmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class VideoCategoriesModelPrivate : public ModelPrivate
{

public:
    VideoCategoriesModelPrivate(VideoCategoriesModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(VideoCategoriesModel);
    
        if (request->status() == VideoCategoriesRequest::Ready) {
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
        
        VideoCategoriesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    VideoCategoriesRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(VideoCategoriesModel)
};

/*!
    \class VideoCategoriesModel
    \brief A list model for displaying YouTube videoCategory resources.
    
    \ingroup videocategories
    \ingroup models
    
    The VideoCategoriesModel is a list model used for displaying YouTube videoCategory resources in a list view. 
    VideoCategoriesModel provides the same methods that are available in VideoCategoriesRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    VideoCategoriesModel provides the following roles and role names:
    
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
    VideoCategoriesModel *model = new VideoCategoriesModel(this);
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    
    QVariantMap filters;
    filters["regionCode"] = "GB";
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
        model: VideoCategoriesModel {
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
        
        Component.onCompleted: sectionsModel.list(["snippet"], {regionCode: "GB"})
    }
    \endcode
    
    \sa VideoCategoriesRequest
*/

/*!
    \enum VideoCategoriesModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
VideoCategoriesModel::VideoCategoriesModel(QObject *parent) :
    Model(*new VideoCategoriesModelPrivate(this), parent)
{
    Q_D(VideoCategoriesModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new VideoCategoriesRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString VideoCategoriesModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa VideoCategoriesRequest::apiKey
*/

/*!
    \fn void VideoCategoriesModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString VideoCategoriesModel::apiKey() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->apiKey();
}

void VideoCategoriesModel::setApiKey(const QString &key) {
    Q_D(VideoCategoriesModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString VideoCategoriesModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa VideoCategoriesRequest::clientId
*/

/*!
    \fn void VideoCategoriesModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString VideoCategoriesModel::clientId() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->clientId();
}

void VideoCategoriesModel::setClientId(const QString &id) {
    Q_D(VideoCategoriesModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString VideoCategoriesModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa VideoCategoriesRequest::clientSecret
*/

/*!
    \fn void VideoCategoriesModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString VideoCategoriesModel::clientSecret() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->clientSecret();
}

void VideoCategoriesModel::setClientSecret(const QString &secret) {
    Q_D(VideoCategoriesModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString VideoCategoriesModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa VideoCategoriesRequest::accessToken
*/

/*!
    \fn void VideoCategoriesModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString VideoCategoriesModel::accessToken() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->accessToken();
}

void VideoCategoriesModel::setAccessToken(const QString &token) {
    Q_D(VideoCategoriesModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString VideoCategoriesModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa VideoCategoriesRequest::refreshToken
*/

/*!
    \fn void VideoCategoriesModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString VideoCategoriesModel::refreshToken() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->refreshToken();
}

void VideoCategoriesModel::setRefreshToken(const QString &token) {
    Q_D(VideoCategoriesModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum VideoCategoriesModel::status
    \brief The current status of the model.
    
    \sa VideoCategoriesRequest::status
*/

/*!
    \fn void VideoCategoriesModel::statusChanged()
    \brief Emitted when the status changes.
*/
VideoCategoriesRequest::Status VideoCategoriesModel::status() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->status();
}

/*!
    \property enum VideoCategoriesModel::error
    \brief The error type of the model.
    
    \sa VideoCategoriesRequest::error
*/
VideoCategoriesRequest::Error VideoCategoriesModel::error() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->error();
}

/*!
    \property enum VideoCategoriesModel::errorString
    \brief A description of the error of the model.
    
    \sa VideoCategoriesRequest::status
*/
QString VideoCategoriesModel::errorString() const {
    Q_D(const VideoCategoriesModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    VideoCategoriesModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa VideoCategoriesRequest::setNetworkAccessManager()
*/
void VideoCategoriesModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(VideoCategoriesModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool VideoCategoriesModel::canFetchMore(const QModelIndex &) const {
    if (status() == VideoCategoriesRequest::Loading) {
        return false;
    }
    
    Q_D(const VideoCategoriesModel);
    
    return !d->nextPageToken.isEmpty();
}

void VideoCategoriesModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(VideoCategoriesModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube videoCategory resources.
    
    \sa VideoCategoriesRequest::list()
*/
void VideoCategoriesModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != VideoCategoriesRequest::Loading) {
        Q_D(VideoCategoriesModel);
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
    \brief Cancels the current request.
    
    \sa VideoCategoriesRequest::cancel()
*/
void VideoCategoriesModel::cancel() {
    Q_D(VideoCategoriesModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube videoCategory resources using the 
    existing parameters.
*/
void VideoCategoriesModel::reload() {
    if (status() != VideoCategoriesRequest::Loading) {
        Q_D(VideoCategoriesModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_videocategoriesmodel.cpp"
