/*************************************************************************************************
 *                                                                                                *
 *  file: filmtechinfoview.cpp                                                                    *
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

#include "filmtechinfoview.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"
#include "tools/mediainfo.h"

#include <QAbstractProxyModel>
#include <QMutexLocker>
#include <QResizeEvent>
#include <thread>


FilmTechInfoView::FilmTechInfoView( QWidget* parent ) : QLabel( parent ),
    settings( AlexandraSettings::GetInstance() )
{
    connect( this, &FilmTechInfoView::ShortInfoLoaded, this, &FilmTechInfoView::ShowShortInfo );
}


FilmTechInfoView::~FilmTechInfoView()
{
      // Behavior on program exit while data loading
      // Wait for 5 seconds, must be suffice
    if( mutexInfoLoad.tryLock( 5000 ) )
    {
        mutexInfoLoad.unlock();
    }
}


void FilmTechInfoView::ShowInformation( const QModelIndex& index )
{
    if( index.isValid() )
    {
        if( settings->GetShowTechInfo() && settings->GetMainWindowShowRightPanel() )
        {
            const QAbstractProxyModel* model = static_cast<const QAbstractProxyModel*>( index.model() );
            FilmItem* film = static_cast<FilmItem*>( model->mapToSource(index).internalPointer() );

            if( film->GetIsFileExists() )
            {
                std::thread( &FilmTechInfoView::LoadTechnicalInfo, this, film->GetFileName() ).detach();
            }
            else
            {
                // TODO: maybe need to show 'loading' image?
                Clear();
            }
        }
    }
}


void FilmTechInfoView::resizeEvent( QResizeEvent* event )
{
    if( event->oldSize().width() != event->size().width() )
    {
        Clear();
    }
}


void FilmTechInfoView::showEvent( QShowEvent* event )
{
    QLabel::showEvent( event );
    Clear();
}


void FilmTechInfoView::LoadTechnicalInfo( const QString& fileName )
{
    #ifdef MEDIAINFO_SUPPORT
        DebugPrintFunc( "FilmTechInfoView::LoadTechnicalInfo", fileName );
        QMutexLocker locker( &mutexInfoLoad );///
        MediaInfo* mi = new MediaInfo( fileName );

          // Filling
        QString shortInfo = QString( "%1 &bull; %2 &bull; %3<br/>" ).arg( mi->GetFormat(),
                                                                          mi->GetFileSize(),
                                                                          mi->GetOverallBitRate() );

        shortInfo += tr( "%1&times;%2 px &bull; %3 fps<br/>" ).arg( mi->GetWidth(), // TRANSLATORS: Translate only "px" and "fps"
                                                                    mi->GetHeight(),
                                                                    mi->GetFrameRate() );

        shortInfo += tr( "Duration &mdash; %1" ).arg( mi->GetDuration() ); // TRANSLATORS: Translate only "Duration"

        DebugPrintFuncDone( "FilmTechInfoView::LoadTechnicalInfo" );
        emit ShortInfoLoaded( shortInfo );
        delete mi;
    #endif // MEDIAINFO_SUPPORT
}

