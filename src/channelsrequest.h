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
 
#ifndef CHANNELSREQUEST_H
#define CHANNELSREQUEST_H

#include "request.h"

namespace QYouTube {

class QYOUTUBESHARED_EXPORT ChannelsRequest : public Request
{
    Q_OBJECT
    
public:
    explicit ChannelsRequest(QObject *parent = 0);
    
public Q_SLOTS:
    void list(const QStringList &part, const QVariantMap &fiters, const QVariantMap &params = QVariantMap());
    
    void update(const QVariantMap &resource, const QStringList &part, const QVariantMap &params = QVariantMap());
    
private:
    Q_DISABLE_COPY(ChannelsRequest)
};

}

#endif // CHANNELSREQUEST_H
