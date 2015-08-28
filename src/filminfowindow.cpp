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

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QtConcurrentRun>

FilmInfoWindow::FilmInfoWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    connect( this, &FilmInfoWindow::FullInfoLoaded, this, &FilmInfoWindow::ShowFullInfo );
    connect( bCopyToClipboard, &QPushButton::clicked, this, &FilmInfoWindow::CopyToClipboard );
}

FilmInfoWindow::~FilmInfoWindow()
{
    // Behavior on program exit while data loading
    if( loadInfoMutex.tryLock( 5000 ) )  // 5 seconds
    {
        loadInfoMutex.unlock();
    }
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
    QString shortInfo = QString( "%1 &bull; %2 &bull; %3<br/>" ).arg( mi->GetFormat(),
                                                                      mi->GetFileSize(),
                                                                      mi->GetOverallBitRate() );
    shortInfo += tr( "%1&times;%2 px &bull; %3 fps<br/>" ).arg( mi->GetWidth(), // TRANSLATORS: Translate only "px" and "fps"
                                                                mi->GetHeight(),
                                                                mi->GetFrameRate() );
    shortInfo += tr( "Duration &mdash; %1" ).arg( mi->GetDuration() ); // TRANSLATORS: Translate only "Duration"

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

void FilmInfoWindow::CopyToClipboard()
{
    eTechInfo->selectAll();
    eTechInfo->copy();
    QMessageBox::information( this, tr( "Technical information" ), tr( "Successfully copied." ) );
}
