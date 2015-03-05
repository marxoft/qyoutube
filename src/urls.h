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

#ifndef URLS_H
#define URLS_H

namespace QYouTube {

// Authentication
#define AUTH_URL "https://accounts.google.com/o/oauth2/auth"
#define TOKEN_URL "https://accounts.google.com/o/oauth2/token"
#define DEVICE_CODE_URL "https://accounts.google.com/o/oauth2/device/code"
#define REVOKE_TOKEN_URL "https://accounts.google.com/o/oauth2/revoke"
#define REDIRECT_URI "urn:ietf:wg:oauth:2.0:oob"
#define READ_ONLY_SCOPE "https://www.googleapis.com/auth/youtube.readonly"
#define READ_WRITE_SCOPE "https://www.googleapis.com/auth/youtube"
#define UPLOAD_SCOPE "https://www.googleapis.com/auth/youtube.upload"
#define AUDIT_SCOPE "https://www.googleapis.com/auth/youtubepartner-channel-audit"

// Activities
#define ACTIVITIES_URL "https://www.googleapis.com/youtube/v3/activities"

// Channels
#define CHANNELS_URL "https://www.googleapis.com/youtube/v3/channels"

// ChannelSections
#define CHANNEL_SECTIONS_URL "https://www.googleapis.com/youtube/v3/channelSections"

// GuideCategories
#define GUIDE_CATEGORIES_URL "https://www.googleapis.com/youtube/v3/guideCategories"

// i18nLanguages
#define I18N_LANGUAGES_URL "https://www.googleapis.com/youtube/v3/i18nLanguages"

// i18nRegions
#define I18N_REGIONS_URL "https://www.googleapis.com/youtube/v3/i18nRegions"

// PlaylistItems
#define PLAYLIST_ITEMS_URL "https://www.googleapis.com/youtube/v3/playlistItems"

// Playlists
#define PLAYLISTS_URL "https://www.googleapis.com/youtube/v3/playlists"

// Search
#define SEARCH_URL "https://www.googleapis.com/youtube/v3/search"

// Subscriptions
#define SUBSCRIPTIONS_URL "https://www.googleapis.com/youtube/v3/subscriptions"

// Subtitles
#define SUBTITLES_URL "https://video.google.com/timedtext"

// VideoCategories
#define VIDEO_CATEGORIES_URL "https://www.googleapis.com/youtube/v3/videoCategories"

// VideoInfo
#define VIDEO_INFO_URL "https://www.youtube.com/get_video_info"

// VideoPage
#define VIDEO_PAGE_URL "https://www.youtube.com/watch"

// Videos
#define VIDEOS_URL "https://www.googleapis.com/youtube/v3/videos"

}

#endif // URLS_H
