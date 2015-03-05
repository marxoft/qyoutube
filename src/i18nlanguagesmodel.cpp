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

#include "i18nlanguagesmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class I18nLanguagesModelPrivate : public ModelPrivate
{

public:
    I18nLanguagesModelPrivate(I18nLanguagesModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(I18nLanguagesModel);
    
        if (request->status() == I18nLanguagesRequest::Ready) {
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
        
        I18nLanguagesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    I18nLanguagesRequest *request;
    
    QStringList part;
    QVariantMap params;
        
    QString previousPageToken;
    QString nextPageToken;
    
    Q_DECLARE_PUBLIC(I18nLanguagesModel)
};

/*!
    \class I18nLanguagesModel
    \brief A list model for displaying YouTube i18nLanguage resources.
    
    \ingroup i18nlanguages
    \ingroup models
    
    The I18nLanguagesModel is a list model used for displaying YouTube i18nLanguage resources in a list view. 
    I18nLanguagesModel provides the same methods that are available in I18nLanguagesRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    I18nLanguagesModel provides the following roles and role names:
    
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
    I18nLanguagesModel *model = new I18nLanguagesModel(this);
    model->setApiKey(MY_API_KEY);
    view->setModel(model);
    model->list(QStringList() << "snippet");
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QYouTube 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: I18nLanguagesModel {
            id: languagesModel
            
            apiKey: MY_API_KEY
        }
        delegate: Text {
            width: view.width
            height: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: snippet.name
        }
        
        Component.onCompleted: languagesModel.list(["snippet"])
    }
    \endcode
    
    \sa I18nLanguagesRequest
*/

/*!
    \enum I18nLanguagesModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
I18nLanguagesModel::I18nLanguagesModel(QObject *parent) :
    Model(*new I18nLanguagesModelPrivate(this), parent)
{
    Q_D(I18nLanguagesModel);
    d->roles[KindRole] = "kind";
    d->roles[EtagRole] = "etag";
    d->roles[IdRole] = "id";
    d->roles[SnippetRole] = "snippet";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new I18nLanguagesRequest(this);
    connect(d->request, SIGNAL(apiKeyChanged()), this, SIGNAL(apiKeyChanged()));
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged()), this, SIGNAL(accessTokenChanged()));
    connect(d->request, SIGNAL(refreshTokenChanged()), this, SIGNAL(refreshTokenChanged()));
}

/*!
    \property QString I18nLanguagesModel::apiKey
    \brief The api key to be used when making requests to the YouTube Data API.
    
    \sa I18nLanguagesRequest::apiKey
*/

/*!
    \fn void I18nLanguagesModel::apiKeyChanged()
    \brief Emitted when the apiKey changes.
*/
QString I18nLanguagesModel::apiKey() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->apiKey();
}

void I18nLanguagesModel::setApiKey(const QString &key) {
    Q_D(I18nLanguagesModel);
    
    d->request->setApiKey(key);
}

/*!
    \property QString I18nLanguagesModel::clientId
    \brief The client id to be used when making requests to the YouTube Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa I18nLanguagesRequest::clientId
*/

/*!
    \fn void I18nLanguagesModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString I18nLanguagesModel::clientId() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->clientId();
}

void I18nLanguagesModel::setClientId(const QString &id) {
    Q_D(I18nLanguagesModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString I18nLanguagesModel::clientSecret
    \brief The client secret to be used when making requests to the YouTube Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa I18nLanguagesRequest::clientSecret
*/

/*!
    \fn void I18nLanguagesModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString I18nLanguagesModel::clientSecret() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->clientSecret();
}

void I18nLanguagesModel::setClientSecret(const QString &secret) {
    Q_D(I18nLanguagesModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString I18nLanguagesModel::accessToken
    \brief The access token to be used when making requests to the YouTube Data API.
    
    The access token is required when accessing a user's protected resources.
    
    \sa I18nLanguagesRequest::accessToken
*/

/*!
    \fn void I18nLanguagesModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString I18nLanguagesModel::accessToken() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->accessToken();
}

void I18nLanguagesModel::setAccessToken(const QString &token) {
    Q_D(I18nLanguagesModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString I18nLanguagesModel::refreshToken
    \brief The refresh token to be used when making requests to the YouTube Data API.
    
    The refresh token is used only when the access token needs to be refreshed.
    
    \sa I18nLanguagesRequest::refreshToken
*/

/*!
    \fn void I18nLanguagesModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString I18nLanguagesModel::refreshToken() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->refreshToken();
}

void I18nLanguagesModel::setRefreshToken(const QString &token) {
    Q_D(I18nLanguagesModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum I18nLanguagesModel::status
    \brief The current status of the model.
    
    \sa I18nLanguagesRequest::status
*/

/*!
    \fn void I18nLanguagesModel::statusChanged()
    \brief Emitted when the status changes.
*/
I18nLanguagesRequest::Status I18nLanguagesModel::status() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->status();
}

/*!
    \property enum I18nLanguagesModel::error
    \brief The error type of the model.
    
    \sa I18nLanguagesRequest::error
*/
I18nLanguagesRequest::Error I18nLanguagesModel::error() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->error();
}

/*!
    \property enum I18nLanguagesModel::errorString
    \brief A description of the error of the model.
    
    \sa I18nLanguagesRequest::status
*/
QString I18nLanguagesModel::errorString() const {
    Q_D(const I18nLanguagesModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the YouTube Data API.
    
    I18nLanguagesModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa I18nLanguagesRequest::setNetworkAccessManager()
*/
void I18nLanguagesModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(I18nLanguagesModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool I18nLanguagesModel::canFetchMore(const QModelIndex &) const {
    if (status() == I18nLanguagesRequest::Loading) {
        return false;
    }
    
    Q_D(const I18nLanguagesModel);
    
    return !d->nextPageToken.isEmpty();
}

void I18nLanguagesModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(I18nLanguagesModel);
        
        QVariantMap params = d->params;
        params["pageToken"] = d->nextPageToken;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, params);
    }
}

/*!
    \brief Retrieves a list of YouTube i18nLanguage resources.
    
    \sa I18nLanguagesRequest::list()
*/
void I18nLanguagesModel::list(const QStringList &part, const QVariantMap &params) {
    if (status() != I18nLanguagesRequest::Loading) {
        Q_D(I18nLanguagesModel);
        d->part = part;
        d->params = params;
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(part, params);
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa I18nLanguagesRequest::cancel()
*/
void I18nLanguagesModel::cancel() {
    Q_D(I18nLanguagesModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of YouTube i18nLanguage resources using the 
    existing parameters.
*/
void I18nLanguagesModel::reload() {
    if (status() != I18nLanguagesRequest::Loading) {
        Q_D(I18nLanguagesModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->part, d->params);
        emit statusChanged();
    }
}

}

#include "moc_i18nlanguagesmodel.cpp"
