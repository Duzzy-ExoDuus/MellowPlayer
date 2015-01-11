//---------------------------------------------------------
//
// This file is part of MellowPlayer.
//
// MellowPlayer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MellowPlayer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MellowPlayer.  If not, see <http://www.gnu.org/licenses/>.
//
//---------------------------------------------------------
#include <QObject>
#include "mellowplayer/song.h"


//---------------------------------------------------------
QString playbackStatusToString(PlaybackStatus status, bool allowLoading)
{
    switch (status) {
    case Loading:
        if(allowLoading)
            return QObject::tr("Loading");
        else
            return QObject::tr("Playing");
    case Playing:
        return QObject::tr("Playing");
    case Paused:
        return QObject::tr("Paused");
    case Stopped:
        return QObject::tr("Stopped");
    }
    return "";
}

//---------------------------------------------------------
bool SongInfo::isValid() const
{
    return songName != "";
}

//---------------------------------------------------------
QString SongInfo::toString() const
{
    return QObject::tr("%1 by %2 from %3").arg(songName).arg(artistName).arg(albumName);
}