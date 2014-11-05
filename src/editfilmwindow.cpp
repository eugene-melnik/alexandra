#include "editfilmwindow.h"

#include <QProcessEnvironment>

EditFilmWindow::EditFilmWindow( QWidget* parent ) : AddFilmWindow( parent )
{
    setWindowTitle( tr( "Edit film" ) );
}

void EditFilmWindow::show( const Film* f )
{
    QString posterFileName( "alexandra-poster.png" );

#if defined( Q_OS_LINUX )
    posterFileName = "/tmp/" + posterFileName;
#elif defined( Q_OS_WIN32 )
    posterFileName = QProcessEnvironment::systemEnvironment().value( "TEMP" ) + "\\" + posterFileName;
#endif

    if( !f->GetPoster().save( posterFileName ) ) {
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
