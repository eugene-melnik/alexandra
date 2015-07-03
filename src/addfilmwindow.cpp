/*************************************************************************************************
 *                                                                                                *
 *  file: addfilmwindow.cpp                                                                       *
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

#include "addfilmwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QCryptographicHash>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>

AddFilmWindow::AddFilmWindow( AlexandraSettings* s, QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    bOpenFile->setFocus();

    settings = s;

    connect( bOpenFile, &QPushButton::clicked, this, &AddFilmWindow::OpenFilmFileClicked );
    connect( bOpenPoster, &QPushButton::clicked, this, &AddFilmWindow::OpenPosterFileClicked );
    connect( buttonBox, &QDialogButtonBox::accepted, this, &AddFilmWindow::OkButtonClicked );
}

void AddFilmWindow::showEvent( QShowEvent* event)
{
    filmId = QString( QCryptographicHash::hash( QByteArray::number( qrand() ), QCryptographicHash::Sha1 ).toHex() );
    ClearFields();
    event->accept();
}

void AddFilmWindow::OpenFilmFileClicked()
{
    QString lastFilmPath = settings->GetLastFilmPath();

    QFileInfo fileName = QFileDialog::getOpenFileName( this,
                         tr( "Select film" ),
                         lastFilmPath,
                         tr( "Video files (*.avi *.flv *.m2ts *.m4v *.mkv *.mov *.mp4 *.mpeg *.mpg *.mts *.ogm *.ogv *.rm *.ts *.wmv)" ) );

    if( fileName.isFile() ) {
        eFilmFileName->setText( fileName.absoluteFilePath() );
        eTitle->setText( fileName.baseName() );

        settings->SetLastFilmPath( fileName.absolutePath() );
        settings->sync();
    }
}

void AddFilmWindow::OpenPosterFileClicked()
{
    QString lastPosterPath = settings->GetLastPosterPath();

    QFileInfo fileName = QFileDialog::getOpenFileName( this,
                         tr( "Select image" ),
                         lastPosterPath,
                         tr( "Images (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm)" ) );

    if( fileName.isFile() ) {
        ePosterFileName->setText( fileName.absoluteFilePath() );

        settings->SetLastPosterPath( fileName.absolutePath() );
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
    f.SetId( filmId );
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
    f.SetTags( eTags->text() );
    f.SetIsViewed( cIsViewed->isChecked() );
    f.SetIsFavourite( cIsFavourite->isChecked() );

    // Manipulations with poster
    QString posterFileName = ePosterFileName->text();

    if( !posterFileName.isEmpty() )
    {
        QString postersDir = settings->GetPostersDirPath();
        int newHeight = settings->GetScalePosterToHeight();

        if( !( QFileInfo( ePosterFileName->text() ).absolutePath() == postersDir ) )
        {
            // Creating posters' directory if not exists
            if( !QDir().exists( postersDir ) ) {
                QDir().mkdir( postersDir );
            }

            QPixmap p( posterFileName );

            // Scale to height
            if( (newHeight != 0) && (newHeight < p.height()) ) {
                p = p.scaledToHeight( newHeight, Qt::SmoothTransformation );
            }

            // Move to posters' folder
            QString newPosterFileName = postersDir + "/" + f.GetPosterName();

            if( !p.save( newPosterFileName ) )
            {
                f.SetIsPosterExists( false );
                emit PosterMovingError();
            }
        }

        f.SetIsPosterExists( true );
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
    eTags->clear();
    cIsViewed->setChecked( false );
    cIsFavourite->setChecked( false );
}
