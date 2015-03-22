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
 
#ifndef QYOUTUBE_AUTHENTICATIONREQUEST_H
#define QYOUTUBE_AUTHENTICATIONREQUEST_H

#include "request.h"

class QStringList;

namespace QYouTube {

class AuthenticationRequestPrivate;

class QYOUTUBESHARED_EXPORT AuthenticationRequest : public Request
{
    Q_OBJECT
    
    Q_PROPERTY(QStringList scopes READ scopes WRITE setScopes NOTIFY scopesChanged)
    
public:
    explicit AuthenticationRequest(QObject *parent = 0);
    
    QStringList scopes() const;
    void setScopes(const QStringList &s);
    
public Q_SLOTS:
    void exchangeCodeForAccessToken(const QString &code);
    
    void requestAuthorizationCode();
    
    void revokeAccessToken();
    
Q_SIGNALS:
    void scopesChanged();
    void authorizationCodeReady(const QVariantMap &code);

private:
    Q_DECLARE_PRIVATE(AuthenticationRequest)
    Q_DISABLE_COPY(AuthenticationRequest)
    
    Q_PRIVATE_SLOT(d_func(), void _q_pollForDeviceToken())
};

}

#endif // QYOUTUBE_AUTHENTICATIONREQUEST_H
