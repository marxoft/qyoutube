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

#ifndef QYOUTUBE_URLS_H
#define QYOUTUBE_URLS_H

#include <QString>

namespace QYouTube {

// API
static const QString API_URL("https://www.googleapis.com/youtube/v3");

// Authentication
static const QString AUTH_URL("https://accounts.google.com/o/oauth2/auth");
static const QString TOKEN_URL("https://accounts.google.com/o/oauth2/token");
static const QString DEVICE_CODE_URL("https://accounts.google.com/o/oauth2/device/code");
static const QString REVOKE_TOKEN_URL("https://accounts.google.com/o/oauth2/revoke");
static const QString REDIRECT_URI("urn:ietf:wg:oauth:2.0:oob");

static const QString READ_ONLY_SCOPE("https://www.googleapis.com/auth/youtube.readonly");
static const QString READ_WRITE_SCOPE("https://www.googleapis.com/auth/youtube");
static const QString FORCE_SSL_SCOPE("https://www.googleapis.com/auth/youtube.force-ssl");
static const QString UPLOAD_SCOPE("https://www.googleapis.com/auth/youtube.upload");
static const QString PARTNER_SCOPE("https://www.googleapis.com/auth/youtubepartner");
static const QString AUDIT_SCOPE("https://www.googleapis.com/auth/youtubepartner-channel-audit");

// Subtitles
static const QString SUBTITLES_URL("https://video.google.com/timedtext");

// VideoInfo
static const QString VIDEO_INFO_URL("https://www.youtube.com/get_video_info");

// VideoPage
static const QString VIDEO_PAGE_URL("https://www.youtube.com/watch");

}

#endif // QYOUTUBE_URLS_H
