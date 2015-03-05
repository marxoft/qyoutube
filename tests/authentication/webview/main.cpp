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

#include "webview.h"
#if QT_VERSION >= 0x050000
#include <QGuiApplication>
#else
#include <QApplication>
#endif

int main(int argc, char *argv[]) {
#if QT_VERSION >= 0x050000
    QGuiApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    app.setOrganizationName("QYouTube");
    app.setApplicationName("QYouTube");
    
    WebView view;
    view.resize(800, 600);
    view.show();

    return app.exec();
}
