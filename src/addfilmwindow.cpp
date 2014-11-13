#include "addfilmwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

AddFilmWindow::AddFilmWindow( QSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    setupUi( this );
    bOpenFile->setFocus();

    connect( bOpenFile, SIGNAL( clicked() ), this, SLOT( OpenFilmFileClicked() ) );
    connect( bOpenPoster, SIGNAL( clicked() ), this, SLOT( OpenPosterFileClicked() ) );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );
}

void AddFilmWindow::showEvent( QShowEvent* event)
{
    ClearFields();
    event->accept();
}

void AddFilmWindow::OpenFilmFileClicked()
{
    QString lastFilmPath = settings->value( "Application/LastFilmPath", "." ).toString();

    QFileInfo fileName = QFileDialog::getOpenFileName( this,
                         tr( "Select film" ),
                         lastFilmPath,
                         tr( "Video files (*.avi *.flv *.m2ts *.m4v *.mkv *.mov *.mp4 *.mpeg *.mpg *.mts *.ogm *.ogv *.rm *.ts *.wmv)" ) );

    if( fileName.isFile() ) {
        eFilmFileName->setText( fileName.absoluteFilePath() );
        eTitle->setText( fileName.baseName() );

        settings->setValue( "Application/LastFilmPath", fileName.absolutePath() );
        settings->sync();
    }
}

void AddFilmWindow::OpenPosterFileClicked()
{
    QString lastPosterPath = settings->value( "Application/LastPosterPath", "." ).toString();

    QFileInfo fileName = QFileDialog::getOpenFileName( this,
                         tr( "Select image" ),
                         lastPosterPath,
                         tr( "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)" ) );

    if( fileName.isFile() ) {
        ePosterFileName->setText( fileName.absoluteFilePath() );

        settings->setValue( "Application/LastPosterPath", fileName.absolutePath() );
        settings->sync();
    }
}

void AddFilmWindow::OkButtonClicked()
{
    if( eFilmFileName->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Adding film" ), tr( "You must choose file on the disk." ) );
        eFilmFileName->setFocus();
        return;
    }
    if( eTitle->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Adding film" ), tr( "Field \"Title\" can't be empty." ) );
        eTitle->setFocus();
        return;
    }

    Film f;
    f.SetFileName( eFilmFileName->text() );
    f.SetPosterFromFile( ePosterFileName->text() );
    f.SetTitle( eTitle->text() );
    f.SetOriginalTitle( eOriginalTitle->text() );
    f.SetTagline( eTagline->text() );
    f.SetYearFromStr( eYear->text() );
    f.SetCountry( eCountry->text() );
    f.SetGenre( eGenre->text() );
    f.SetRatingFromStr( cbRating->currentText() );
    f.SetDirector( eDirector->text() );
    f.SetProducer( eProducer->text() );
    f.SetStarring( tStarring->toPlainText() );
    f.SetDescription( tDescription->toPlainText() );

    close();
    emit Done( f );
}

void AddFilmWindow::ClearFields()
{
    eFilmFileName->clear();
    ePosterFileName->clear();
    eTitle->clear();
    eOriginalTitle->clear();
    eTagline->clear();
    eYear->clear();
    eCountry->clear();
    eGenre->clear();
    eDirector->clear();
    eProducer->clear();
    tStarring->clear();
    tDescription->clear();
}
