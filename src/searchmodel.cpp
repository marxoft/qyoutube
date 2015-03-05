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

#include "searchmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class SearchModelPrivate : public ModelPrivate
{

public:
    SearchModelPrivate(SearchModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(SearchModel);
    
        if (request->status() == SearchRequest::Ready) {
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
        
        SearchModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    SearchRequest *request;
    
    QStringList part;
    QVariantMap filters;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(SearchModel)
};

/*!
    \class SearchModel
    \brief A list model for displaying YouTube search results.
    
    \ingroup i18nlanguages
    \ingroup models
    
    The SearchModel is a list model used for displaying YouTube search results in a list view. 
    SearchModel provides the same methods that are available in SearchRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    SearchModel provides the following roles and role names:
    
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
    SearchModel *model = new SearchModel(this);
    QVariantMap params;
    params["type"] = "video";
    params["order"] = "date";
    params["q"] = "Qt";
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    model->list(QStringList() << "snippet", QVariantMap(), params);
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QYouTube 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: SearchModel {
            id: searchModel
            
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
        
        Component.onCompleted: searchModel.list(["snippet"], {}, {type: "video", order: "date", q: "Qt"})
    }
    \endcode
    
    \sa SearchRequest
*/

/*!
    \enum SearchModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
SearchModel::SearchModel(QObject *parent) :
    Model(*new SearchModelPrivate(this), parent)
{
    Q_D(SearchModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new SearchRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString SearchModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa SearchRequest::apiKey
*/

/*!
    \fn void SearchModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString SearchModel::apiKey() const {
    Q_D(const SearchModel);
    
    return d->request->apiKey();
}

void SearchModel::setApiKey(const QString &key) {
    Q_D(SearchModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString SearchModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa SearchRequest::clientId
*/

/*!
    \fn void SearchModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString SearchModel::clientId() const {
    Q_D(const SearchModel);
    
    return d->request->clientId();
}

void SearchModel::setClientId(const QString &id) {
    Q_D(SearchModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString SearchModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa SearchRequest::clientSecret
*/

/*!
    \fn void SearchModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString SearchModel::clientSecret() const {
    Q_D(const SearchModel);
    
    return d->request->clientSecret();
}

void SearchModel::setClientSecret(const QString &secret) {
    Q_D(SearchModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString SearchModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa SearchRequest::accessToken
*/

/*!
    \fn void SearchModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString SearchModel::accessToken() const {
    Q_D(const SearchModel);
    
    return d->request->accessToken();
}

void SearchModel::setAccessToken(const QString &token) {
    Q_D(SearchModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString SearchModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa SearchRequest::refreshToken
*/

/*!
    \fn void SearchModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString SearchModel::refreshToken() const {
    Q_D(const SearchModel);
    
    return d->request->refreshToken();
}

void SearchModel::setRefreshToken(const QString &token) {
    Q_D(SearchModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum SearchModel::status
    \brief The current status of the model.
    
    \sa SearchRequest::status
*/

/*!
    \fn void SearchModel::statusChanged()
    \brief Emitted when the status changes.
*/
SearchRequest::Status SearchModel::status() const {
    Q_D(const SearchModel);
    
    return d->request->status();
}

/*!
    \property enum SearchModel::error
    \brief The error type of the model.
    
    \sa SearchRequest::error
*/
SearchRequest::Error SearchModel::error() const {
    Q_D(const SearchModel);
    
    return d->request->error();
}

/*!
    \property enum SearchModel::errorString
    \brief A description of the error of the model.
    
    \sa SearchRequest::status
*/
QString SearchModel::errorString() const {
    Q_D(const SearchModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    SearchModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa SearchRequest::setNetworkAccessManager()
*/
void SearchModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(SearchModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool SearchModel::canFetchMore(const QModelIndex &) const {
    if (status() == SearchRequest::Loading) {
        return false;
    }
    
    Q_D(const SearchModel);
    
    return !d->nextPageToken.isEmpty();
}

void SearchModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(SearchModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, params);
    }
}

/*!
    \brief Retrieves a list of YouTube search results.
    
    \sa SearchRequest::list()
*/
void SearchModel::list(const QStringList &part, const QVariantMap &filters, const QVariantMap &params) {
    if (status() != SearchRequest::Loading) {
        Q_D(SearchModel);
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
    
    \sa SearchRequest::cancel()
*/
void SearchModel::cancel() {
    Q_D(SearchModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube search results using the 
    existing parameters.
*/
void SearchModel::reload() {
    if (status() != SearchRequest::Loading) {
        Q_D(SearchModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->filters, d->params);
        emit statusChanged();
    }
}

}

#include "moc_searchmodel.cpp"
