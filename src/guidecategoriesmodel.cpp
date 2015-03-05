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

#include "guidecategoriesmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class GuideCategoriesModelPrivate : public ModelPrivate
{

public:
    GuideCategoriesModelPrivate(GuideCategoriesModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(GuideCategoriesModel);
    
        if (request->status() == GuideCategoriesRequest::Ready) {
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
        
        GuideCategoriesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    GuideCategoriesRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(GuideCategoriesModel)
};

/*!
    \class GuideCategoriesModel
    \brief A list model for displaying YouTube guideCategory resources.
    
    \ingroup guidecategories
    \ingroup models
    
    The GuideCategoriesModel is a list model used for displaying YouTube guideCategory resources in a list view. 
    GuideCategoriesModel provides the same methods that are available in GuideCategoriesRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    GuideCategoriesModel provides the following roles and role names:
    
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
    GuideCategoriesModel *model = new GuideCategoriesModel(this);
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
        model: GuideCategoriesModel {
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
    
    \sa GuideCategoriesRequest
*/

/*!
    \enum GuideCategoriesModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
GuideCategoriesModel::GuideCategoriesModel(QObject *parent) :
    Model(*new GuideCategoriesModelPrivate(this), parent)
{
    Q_D(GuideCategoriesModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new GuideCategoriesRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString GuideCategoriesModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa GuideCategoriesRequest::apiKey
*/

/*!
    \fn void GuideCategoriesModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString GuideCategoriesModel::apiKey() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->apiKey();
}

void GuideCategoriesModel::setApiKey(const QString &key) {
    Q_D(GuideCategoriesModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString GuideCategoriesModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa GuideCategoriesRequest::clientId
*/

/*!
    \fn void GuideCategoriesModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString GuideCategoriesModel::clientId() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->clientId();
}

void GuideCategoriesModel::setClientId(const QString &id) {
    Q_D(GuideCategoriesModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString GuideCategoriesModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa GuideCategoriesRequest::clientSecret
*/

/*!
    \fn void GuideCategoriesModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString GuideCategoriesModel::clientSecret() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->clientSecret();
}

void GuideCategoriesModel::setClientSecret(const QString &secret) {
    Q_D(GuideCategoriesModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString GuideCategoriesModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa GuideCategoriesRequest::accessToken
*/

/*!
    \fn void GuideCategoriesModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString GuideCategoriesModel::accessToken() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->accessToken();
}

void GuideCategoriesModel::setAccessToken(const QString &token) {
    Q_D(GuideCategoriesModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString GuideCategoriesModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa GuideCategoriesRequest::refreshToken
*/

/*!
    \fn void GuideCategoriesModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString GuideCategoriesModel::refreshToken() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->refreshToken();
}

void GuideCategoriesModel::setRefreshToken(const QString &token) {
    Q_D(GuideCategoriesModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum GuideCategoriesModel::status
    \brief The current status of the model.
    
    \sa GuideCategoriesRequest::status
*/

/*!
    \fn void GuideCategoriesModel::statusChanged()
    \brief Emitted when the status changes.
*/
GuideCategoriesRequest::Status GuideCategoriesModel::status() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->status();
}

/*!
    \property enum GuideCategoriesModel::error
    \brief The error type of the model.
    
    \sa GuideCategoriesRequest::error
*/
GuideCategoriesRequest::Error GuideCategoriesModel::error() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->error();
}

/*!
    \property enum GuideCategoriesModel::errorString
    \brief A description of the error of the model.
    
    \sa GuideCategoriesRequest::status
*/
QString GuideCategoriesModel::errorString() const {
    Q_D(const GuideCategoriesModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    GuideCategoriesModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa GuideCategoriesRequest::setNetworkAccessManager()
*/
void GuideCategoriesModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(GuideCategoriesModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool GuideCategoriesModel::canFetchMore(const QModelIndex &) const {
    if (status() == GuideCategoriesRequest::Loading) {
        return false;
    }
    
    Q_D(const GuideCategoriesModel);
    
    return !d->nextPageToken.isEmpty();
}

void GuideCategoriesModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(GuideCategoriesModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube guideCategory resources.
    
    \sa GuideCategoriesRequest::list()
*/
void GuideCategoriesModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != GuideCategoriesRequest::Loading) {
        Q_D(GuideCategoriesModel);
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
    
    \sa GuideCategoriesRequest::cancel()
*/
void GuideCategoriesModel::cancel() {
    Q_D(GuideCategoriesModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube guideCategory resources using the 
    existing parameters.
*/
void GuideCategoriesModel::reload() {
    if (status() != GuideCategoriesRequest::Loading) {
        Q_D(GuideCategoriesModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_guidecategoriesmodel.cpp"
