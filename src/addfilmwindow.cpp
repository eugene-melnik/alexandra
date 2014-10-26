#include "addfilmwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

AddFilmWindow::AddFilmWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    ConfigureRatingComboBox();
    bOpenFile->setFocus();

    connect( bOpenFile, SIGNAL( clicked() ), this, SLOT( OpenFilmFileClicked() ) );
    connect( bOpenPoster, SIGNAL( clicked() ), this, SLOT( OpenPosterFileClicked() ) );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );
}

void AddFilmWindow::showEvent(QShowEvent *event)
{
    ClearFields();
    event->accept();
}

void AddFilmWindow::OpenFilmFileClicked()
{
    QFileInfo fileName = QFileDialog::getOpenFileName( this,
                         tr( "Select film" ),
                         ".",
                         tr( "Video files (*.avi *.flv *.m2ts *.m4v *.mkv *.mov *.mp4 *.mpeg *.mpg *.mts *.ogm *.ogv *.rm *.ts *.wmv)" ) );

    eFilmFileName->setText( fileName.absoluteFilePath() );
    eTitle->setText( fileName.baseName() );
}

void AddFilmWindow::OpenPosterFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName( this,
                       tr( "Select image" ),
                       ".",
                       tr( "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)" ) );

    ePosterFileName->setText( fileName );
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
    f.fileName = eFilmFileName->text();
    f.poster = QPixmap( ePosterFileName->text() );
    f.title = eTitle->text();
    f.originalTitle = eOriginalTitle->text();
    f.tagline = eTagline->text();
    f.year = eYear->text().toUShort();
    f.country = eCountry->text();
    f.genre = eGenre->text();
    f.rating = cbRating->currentText().toUShort();
    f.director = eDirector->text();
    f.producer = eProducer->text();
    f.starring = tStarring->toPlainText();
    f.description = tDescription->toPlainText();

    close();
    emit AddFilm( f );
}

void AddFilmWindow::ConfigureRatingComboBox()
{
    for( long i = 10; i >= 1; i-- )
    {
        cbRating->addItem( QString::number( i ) );
    }
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
    cbRating->setCurrentIndex(1);
    eDirector->clear();
    eProducer->clear();
    tStarring->clear();
    tDescription->clear();
}
