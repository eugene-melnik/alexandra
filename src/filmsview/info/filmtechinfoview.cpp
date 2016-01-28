
#include "filmtechinfoview.h"
#include "filmslist/filmitem.h"
#include "tools/debug.h"
#include "mediainfo.h"

#include <QAbstractProxyModel>
#include <QMutexLocker>
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
    if( settings->GetShowTechInfo() && settings->GetMainWindowShowRightPanel() )
    {
        const QAbstractProxyModel* model = static_cast<const QAbstractProxyModel*>( index.model() );
        FilmItem* film = static_cast<FilmItem*>( model->mapToSource(index).internalPointer() );

        if( film->GetIsFileExists() == FilmItem::Exists )
        {
            QString fileName = film->GetColumnData( FilmItem::FileNameColumn ).toString();
            std::thread( &FilmTechInfoView::LoadTechnicalInfo, this, fileName ).detach();
        }
        else
        {
            // TODO: maybe need to show 'loading' image?
            Clear();
        }
    }
}


void FilmTechInfoView::showEvent( QShowEvent* event )
{
    QLabel::showEvent( event );
    Clear();
}


void FilmTechInfoView::LoadTechnicalInfo( const QString& fileName )
{
    DebugPrintFuncA( "FilmTechInfoView::LoadTechnicalInfo", fileName );

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
}

