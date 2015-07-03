/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowindow.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
 *                                                                                                *
 *  Alexandra is free software; you can redistribute it and/or modify it under the terms of the   *
 *  GNU General Public License as published by the Free Software Foundation; either version 2 of  *
 *  the License, or (at your option) any later version.                                           *
 *                                                                                                *
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     *
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     *
 *  See the GNU General Public License for more details.                                          *
 *                                                                                                *
 *  You should have received a copy of the GNU General Public License along with this program.    *
 *  If not, see <http://www.gnu.org/licenses/>.                                                   *
 *                                                                                                *
  *************************************************************************************************/

#include "filminfowindow.h"

#include <QtConcurrentRun>
#include <QPlainTextEdit>
#include <QPushButton>

FilmInfoWindow::FilmInfoWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    connect( this, &FilmInfoWindow::FullInfoLoaded, this, &FilmInfoWindow::ShowFullInfo );
}

void FilmInfoWindow::LoadTechnicalInfoAsync( const QString& fileName )
{
    eTechInfo->clear();
    QtConcurrent::run( this, &FilmInfoWindow::LoadTechnicalInfo, fileName );
}

void FilmInfoWindow::LoadTechnicalInfo( const QString& fileName )
{
    loadInfoMutex.lock();
    MediaInfo* mi = new MediaInfo( fileName );

    // Short info
    // First line of the Short info
    QString shortInfo = mi->GetFormat() + " <b>|</b> ";
    shortInfo += mi->GetFileSize() + " <b>|</b> ";
    shortInfo += mi->GetOverallBitRate() + "<br/>";
    // Second line...
    shortInfo += mi->GetWidth() + "&times;";
    shortInfo += mi->GetHeight() + " px <b>|</b> ";
    shortInfo += mi->GetFrameRate() + "<br/>";
    // Third line...
    shortInfo += mi->GetDuration();

    emit ShortInfoLoaded( shortInfo );

    // Full info
    emit FullInfoLoaded( mi->GetCompleteData() );

    delete mi;
    loadInfoMutex.unlock();
}

void FilmInfoWindow::ShowFullInfo( QString s )
{
    eTechInfo->setPlainText( s );
}
