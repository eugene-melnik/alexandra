#include "editfilmwindow.h"

#include <QProcessEnvironment>

EditFilmWindow::EditFilmWindow( QSettings* s, QWidget* parent ) : AddFilmWindow( s, parent )
{
    setWindowTitle( tr( "Edit film" ) );
}

void EditFilmWindow::show( const Film* f )
{
    // Magic manipulations with poster image :)
    QString posterFileName( "alexandra-poster.png" );

#if defined( Q_OS_LINUX )
    posterFileName = "/tmp/" + posterFileName;
#elif defined( Q_OS_WIN32 )
    posterFileName = QProcessEnvironment::systemEnvironment().value( "TEMP" ) + "\\" + posterFileName;
#endif

    if( !f->GetPoster().save( posterFileName ) ) { // if poster doesn't exists
        posterFileName.clear();
    }

    QWidget::show();

    eFilmFileName->setText( f->GetFileName() );
    ePosterFileName->setText( posterFileName );
    eTitle->setText( f->GetTitle() );
    eOriginalTitle->setText( f->GetOriginalTitle() );
    eTagline->setText( f->GetTagline() );
    eYear->setText( f->GetYearStr() );
    eCountry->setText( f->GetCountry() );
    eGenre->setText( f->GetGenre() );
    cbRating->setCurrentIndex( f->GetRating() - 1 );
    eDirector->setText( f->GetDirector() );
    eProducer->setText( f->GetProducer() );
    tStarring->setPlainText( f->GetStarring() );
    tDescription->setPlainText( f->GetDescription() );
}
