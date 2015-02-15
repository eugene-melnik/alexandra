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
    connect( this, SIGNAL( FullInfoLoaded(QString) ), this, SLOT( ShowFullInfo(QString) ) );
}

void FilmInfoWindow::SetCurrentFile( const QString& f )
{
    eTechInfo->clear();
    QtConcurrent::run( this, &FilmInfoWindow::LoadInfo, f );
}

void FilmInfoWindow::ShowFullInfo( QString s )
{
    eTechInfo->setPlainText( s );
}

void FilmInfoWindow::LoadInfo( const QString& f )
{
    MediaInfoNameSpace::MediaInfo* mi = new MediaInfoNameSpace::MediaInfo();
    mi->Option( __T( "CharSet" ), __T( "UTF-8" ) );
    mi->Open( MediaInfoNameSpace::String( f.toStdWString() ) );

    // First line of the Short info
    QString shortInfo =  QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_General, 0, __T( "Format" ) ) ) + " | ";
    shortInfo += QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_General, 0, __T( "FileSize/String3" ) ) ) + " | ";
    shortInfo += QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_General, 0, __T( "OverallBitRate/String" ) ) ) + "<br/>";
    // Second line...
    shortInfo += QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_Video, 0, __T( "Width" ) ) ) + "&times;";
    shortInfo += QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_Video, 0, __T( "Height" ) ) ) + " px | ";
    shortInfo += QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_Video, 0, __T( "FrameRate/String" ) ) ) + "<br/>";
    // Third line...
    shortInfo += QString::fromStdWString( mi->Get( MediaInfoNameSpace::Stream_General, 0, __T( "Duration/String" ) ) );
    emit ShortInfoLoaded( shortInfo );

    // Full info
    mi->Option( __T( "Complete" ) );
    emit FullInfoLoaded( QString::fromStdWString( mi->Inform() ) );

    mi->Close();
    delete mi;
}
