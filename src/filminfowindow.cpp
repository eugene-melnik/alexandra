/*************************************************************************************************
 *                                                                                                *
 *  file: filminfowindow.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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
#include "tools/debug.h"

#include <QMessageBox>
#include <QMutexLocker>
#include <thread>


FilmInfoWindow::FilmInfoWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    connect( bCopyToClipboard, &QPushButton::clicked, this, &FilmInfoWindow::CopyToClipboard );
    connect( this, &FilmInfoWindow::FullInfoLoaded, this, [this] (const QString& text){ eTechInfo->setPlainText( text ); } );
}


FilmInfoWindow::~FilmInfoWindow()
{
      // Behavior on program exit while data loading
      // Wait for 5 seconds, must siffice
    if( loadInfoMutex.tryLock( 5000 ) )
    {
        loadInfoMutex.unlock();
    }
}


void FilmInfoWindow::LoadTechnicalInfoAsync( const QString& fileName )
{
    std::thread( &FilmInfoWindow::LoadTechnicalInfo, this, fileName ).detach();
}


void FilmInfoWindow::LoadTechnicalInfo( const QString& fileName )
{
    DebugPrintFunc( "FilmInfoWindow::LoadTechnicalInfo", fileName );
    QMutexLocker locker( &loadInfoMutex );

    MediaInfo* mi = new MediaInfo( fileName );
    emit FullInfoLoaded( mi->GetCompleteData() );
    delete mi;

    DebugPrintFuncDone( "FilmInfoWindow::LoadTechnicalInfo" );
}


void FilmInfoWindow::CopyToClipboard()
{
    eTechInfo->selectAll();
    eTechInfo->copy();
    QMessageBox::information( this, tr("Technical information"), tr("Successfully copied.") );
}

