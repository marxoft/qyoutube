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

#include "subtitlesmodel.h"
#include "model_p.h"
#include <QStringList>

namespace QYouTube {

class SubtitlesModelPrivate : public ModelPrivate
{

public:
    SubtitlesModelPrivate(SubtitlesModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(SubtitlesModel);
    
        if (request->status() == SubtitlesRequest::Ready) {
            QVariantList list = request->result().toList();
        
            if (!list.isEmpty()) {
                q->beginInsertRows(QModelIndex(), items.size(), items.size() + list.size());
                
                foreach (QVariant item, list) {
                    items << item.toMap();
                }
                
                q->endInsertRows();
                emit q->countChanged();
            }
        }
        
        SubtitlesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged();
    }
    
    SubtitlesRequest *request;
    
    QString id;
        
    Q_DECLARE_PUBLIC(SubtitlesModel)
};

/*!
    \class SubtitlesModel
    \brief A list model for displaying list of subtitles for a YouTube video.
    
    \ingroup subtitles
    \ingroup models
    
    The SubtitlesModel is a list model used for displaying YouTube video subtitles in a list view. 
    SubtitlesModel provides the same methods that are available in SubtitlesRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    SubtitlesModel provides the following roles and role names:
    
    <table>
        <tr>
        <th>Role</th>
        <th>Role name</th>
        </tr>
        <tr>
            <td>LanguageRole</td>
            <td>language</td>
        </tr>
        <tr>
            <td>LanguageCodeRole</td>
            <td>languageCode</td>
        </tr>
        <tr>
            <td>UrlRole</td>
            <td>url</td>
        </tr>
    </table>
        
    Example usage:
    
    C++
    
    \code
    using namespace QYouTube;
    
    ...
    
    QListView *view = new QListView(this);
    SubtitlesModel *model = new SubtitlesModel(this);
    view->setModel(model);
    model->list(VIDEO_ID);
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QYouTube 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: SubtitlesModel {
            id: subtitlesModel
        }
        delegate: Text {
            width: view.width
            height: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: language
        }
        
        Component.onCompleted: subtitlesModel.list(VIDEO_ID)
    }
    \endcode
    
    \sa SubtitlesRequest
*/

/*!
    \enum SubtitlesModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
SubtitlesModel::SubtitlesModel(QObject *parent) :
    Model(*new SubtitlesModelPrivate(this), parent)
{
    Q_D(SubtitlesModel);
    d->roles[LanguageRole] = "language";
    d->roles[LanguageCodeRole] = "languageCode";
    d->roles[UrlRole] = "url";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new SubtitlesRequest(this);
}

/*!
    \property enum SubtitlesModel::status
    \brief The current status of the model.
    
    \sa SubtitlesRequest::status
*/

/*!
    \fn void SubtitlesModel::statusChanged()
    \brief Emitted when the status changes.
*/
SubtitlesRequest::Status SubtitlesModel::status() const {
    Q_D(const SubtitlesModel);
    
    return d->request->status();
}

/*!
    \property enum SubtitlesModel::error
    \brief The error type of the model.
    
    \sa SubtitlesRequest::error
*/
SubtitlesRequest::Error SubtitlesModel::error() const {
    Q_D(const SubtitlesModel);
    
    return d->request->error();
}

/*!
    \property enum SubtitlesModel::errorString
    \brief A description of the error of the model.
    
    \sa SubtitlesRequest::status
*/
QString SubtitlesModel::errorString() const {
    Q_D(const SubtitlesModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests.
    
    SubtitlesModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa SubtitlesRequest::setNetworkAccessManager()
*/
void SubtitlesModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(SubtitlesModel);
    
    d->request->setNetworkAccessManager(manager);
}

/*!
    \brief Retrieves a list of subtitles for a YouTube video.
    
    \sa SubtitlesRequest::list()
*/
void SubtitlesModel::list(const QString &id) {
    if (status() != SubtitlesRequest::Loading) {
        Q_D(SubtitlesModel);
        d->id = id;
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(id);
        emit statusChanged();
    }
}

/*!
    \brief Cancels the current request.
    
    \sa SubtitlesRequest::cancel()
*/
void SubtitlesModel::cancel() {
    Q_D(SubtitlesModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of subtitles using the existing parameters.
*/
void SubtitlesModel::reload() {
    if (status() != SubtitlesRequest::Loading) {
        Q_D(SubtitlesModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->list(d->id);
        emit statusChanged();
    }
}

}

#include "moc_subtitlesmodel.cpp"
