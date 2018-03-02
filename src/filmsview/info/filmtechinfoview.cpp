/*************************************************************************************************
 *                                                                                                *
 *  file: filmtechinfoview.cpp                                                                    *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

#include <thread>
#include <QAbstractProxyModel>
#include <QMutexLocker>
#include <QResizeEvent>


FilmTechInfoView::FilmTechInfoView( QWidget* parent ) : QLabel( parent ),
    settings( AlexandraSettings::GetInstance() )
{
    connect( this, &FilmTechInfoView::ShortInfoLoaded, this, &FilmTechInfoView::ShowShortInfo );

    connect( &loadingTimer, &QTimer::timeout, this, [this]
    {
        loadingTimer.stop();
        std::thread( &FilmTechInfoView::LoadTechnicalInfo, this, loadingPath ).detach();
    } );
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
    loadingTimer.stop();

    if( index.isValid() && settings->GetShowTechInfo() && settings->GetMainWindowShowRightPanel() )
    {
        const QAbstractProxyModel* model = static_cast<const QAbstractProxyModel*>( index.model() );
        FilmItem* film = static_cast<FilmItem*>( model->mapToSource(index).internalPointer() );

        if( film->GetIsFileExists() )
        {
            ShowLoading();
            loadingPath = film->GetFileName();
            loadingTimer.start( 200 ); // ms
            return;
        }
    }

    Clear();
}


void FilmTechInfoView::Clear()
{
    setMinimumHeight( 0 );
    clear();
    repaint();
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


void FilmTechInfoView::ShowLoading()
{
    setPixmap( loadPixmap );
    setMinimumHeight( loadPixmap.height() );
}


void FilmTechInfoView::LoadTechnicalInfo( const QString& fileName )
{
    #ifdef MEDIAINFO_SUPPORT
        DebugPrintFunc( "FilmTechInfoView::LoadTechnicalInfo", fileName );
        QMutexLocker locker( &mutexInfoLoad );

        MediaInfo* mi = new MediaInfo( fileName );

          // Filling
        QString shortInfo = QString( "%1 &bull; %2 &bull; %3<br/>" ).arg( mi->GetFormat(),
                                                                          mi->GetFileSize(),
                                                                          mi->GetOverallBitRate() );

        shortInfo += QString( "%1&times;%2 %3 &bull; %4 %5<br/>" ).arg( mi->GetWidth(),
                                                                        mi->GetHeight(),
                                                                        tr("px"),
                                                                        mi->GetFrameRate(),
                                                                        tr("fps") );

        shortInfo += QString( "%1 &mdash; %2" ).arg( tr("Duration"), mi->GetDuration() );

        DebugPrintFuncDone( "FilmTechInfoView::LoadTechnicalInfo" );
        emit ShortInfoLoaded( shortInfo );
        delete mi;
    #endif // MEDIAINFO_SUPPORT
}

