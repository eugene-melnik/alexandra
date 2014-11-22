/*************************************************************************************************
 *                                                                                                *
 *  file: addfilmwindow.cpp                                                                       *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#include "addfilmwindow.h"

#include <QComboBox>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>

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
    // Checking necessary fields
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

    // Text data
    Film f;
    f.SetFileName( eFilmFileName->text() );
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
    f.SetIsViewed( cIsViewed->isChecked() );
    f.SetIsFavourite( cIsFavourite->isChecked() );

    // Manipulations with poster
    QString posterFileName = ePosterFileName->text();
    QString postersFolder = settings->value( "FilmList/PostersFolder" ).toString();
    int newHeight = settings->value( "FilmList/ScalePosters", 0 ).toInt();

    if( QFileInfo( ePosterFileName->text() ).absolutePath() == postersFolder ) {
        // If poster is already in posters' folder
        f.SetPosterName( QFileInfo( posterFileName ).fileName() );
    } else {
        QPixmap p( posterFileName );

        // Scale to height
        if( (newHeight != 0) && (newHeight < p.height()) ) {
            p = p.scaledToHeight( newHeight, Qt::SmoothTransformation );
        }

        // Move to posters' folder
        QString newPosterFileName = postersFolder + "/"
                    + QString( QCryptographicHash::hash( QByteArray::number( qrand() ), QCryptographicHash::Sha1 ).toHex() + ".png" );

        if( p.save( newPosterFileName ) ) {
            f.SetPosterName( QFileInfo( newPosterFileName ).fileName() );
        } else {
            emit PosterMovingError();
        }
    }

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
    cbRating->setCurrentIndex( 0 );
    eDirector->clear();
    eProducer->clear();
    tStarring->clear();
    tDescription->clear();
}
