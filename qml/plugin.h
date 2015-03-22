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

#ifndef QYOUTUBE_PLUGIN_H
#define QYOUTUBE_PLUGIN_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
#include <QQmlExtensionPlugin>
#else
#include <QDeclarativeExtensionPlugin>
#endif

namespace QYouTube {

#if QT_VERSION >= 0x050000
class Plugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    
    Q_PLUGIN_METADATA(IID "org.qyoutube.QYouTubeQmlPlugin")

public:
    void registerTypes(const char *uri);
};
#else
class Plugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
};
#endif

}

#endif // QYOUTUBE_PLUGIN_H
