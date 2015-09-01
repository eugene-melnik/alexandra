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
#include "filesextensions.h"

#include <string>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QStringList>

AddFilmWindow::AddFilmWindow( AlexandraSettings* s, QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    bOpenFile->setFocus();

    settings = s;

    connect( bOpenFile, &QPushButton::clicked, this, &AddFilmWindow::OpenFilm );
    connect( bOpenPoster, &QPushButton::clicked, this, &AddFilmWindow::OpenPosterFileClicked );
    connect( bOk, &QPushButton::clicked, this, &AddFilmWindow::OkButtonClicked );
}

void AddFilmWindow::show()
{
    bOpenPoster->setText( tr( "Open" ) );
    filmId = Film::GetRandomHash();
    QDialog::show();
}

void AddFilmWindow::closeEvent( QCloseEvent* event )
{
    ClearFields();
    event->accept();
}

void AddFilmWindow::OpenFilm()
{
    // Set path to the file depending on the settings
    // or the selected movie (if already selected)
    QString openPath;

    if( eFilmFileName->text().isEmpty() )
    {
        openPath = settings->GetLastFilmPath();
    }
    else
    {
        openPath = QFileInfo( eFilmFileName->text() ).absolutePath();
    }

    // Open file dialog
    QFileInfo fileName = QFileDialog::getOpenFileName( this,
                         tr( "Select film" ),
                         openPath,
                         tr( "Video files (%1)" ).arg( FilesExtensions().GetFilmExtensionsForFilter() ) );

    if( fileName.isFile() )
    {
        // If a file is selected set the file name and the title
        // of the film (with the replacement of characters '_' to spaces)
        eFilmFileName->setText( fileName.absoluteFilePath() );

        if( eTitle->text().isEmpty() )
        {
            eTitle->setText( fileName.completeBaseName().replace( "_", " " ) );
        }

        // Setting the path to the image file, if found in the same directory
        QString posterFileName = FilesExtensions().SearchForEponymousImage( fileName.absoluteFilePath() );

        if( !posterFileName.isEmpty() )
        {
            ePosterFileName->setText( posterFileName );
            bOpenPoster->setText( tr( "Clear" ) );
        }

        settings->SetLastFilmPath( fileName.absolutePath() );
    }
}

void AddFilmWindow::OpenPosterFileClicked()
{
    if( bOpenPoster->text() == tr( "Open" ) ) // If poster isn't selected
    {
        QString lastPosterPath = settings->GetLastPosterPath();

        QFileInfo fileName = QFileDialog::getOpenFileName( this,
                             tr( "Select image" ),
                             lastPosterPath,
                             tr( "Images (%1)" ).arg( FilesExtensions().GetImageExtensionsForFilter() ) );

        if( fileName.isFile() )
        {
            ePosterFileName->setText( fileName.absoluteFilePath() );
            bOpenPoster->setText( tr( "Clear" ) );

            settings->SetLastPosterPath( fileName.absolutePath() );
        }
    }
    else // If poster is already selected
    {
        // Cleaning the filename field and request to delete the file if
        // it has been moved to the directory of posters (for edit film function)
        QString posterPath = QFileInfo( ePosterFileName->text() ).absolutePath();
        QString postersDirPath = settings->GetPostersDirPath();

        if( posterPath == postersDirPath )
        {
            int res = QMessageBox::question( this, tr( "Clear poster" ),
                                                   tr( "Remove image file?" ) );

            if( res == QMessageBox::Yes )
            {
                QFile( ePosterFileName->text() ).remove();

                // Need to update information if poster removed
                ePosterFileName->clear();
                OkButtonClicked();
            }
        }

        ePosterFileName->clear();
        bOpenPoster->setText( tr( "Open" ) );
    }
}

void AddFilmWindow::OkButtonClicked()
{
    // Checking necessary fields
    if( eTitle->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Adding film" ),
                                        tr( "Field \"Title\" can't be empty." ) );
        eTitle->setFocus();
        return;
    }

    // Filling the text data
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
    f.SetBudgetFromStr( eBudget->text() );
    f.SetDescription( tDescription->toPlainText() );
    f.SetTags( eTags->text() );
    f.SetIsViewed( cIsViewed->isChecked() );
    f.SetIsFavourite( cIsFavourite->isChecked() );
    f.SetScreenwriter( eScreenwriter->text() );
    f.SetComposer( eComposer->text() );

    // Manipulations with poster
    QString posterFileName = ePosterFileName->text();

    if( posterFileName.isEmpty() )
    {
        f.SetIsPosterExists( false );
    }
    else
    {
        f.SetIsPosterExists( true );

        QString postersDir = settings->GetPostersDirPath();
        int newHeight = settings->GetScalePosterToHeight();

        if( QFileInfo( posterFileName ).absolutePath() != postersDir )
        {
            // Create a directory for posters, if it does not exist
            if( !QDir().exists( postersDir ) )
            {
                QDir().mkdir( postersDir );
            }

            QPixmap p( posterFileName );

            // Move to directory of the posters with selected format and quality
            // and scaling if necessary
            if( newHeight != 0 && newHeight < p.height() )
            {
                p = p.scaledToHeight( newHeight, Qt::SmoothTransformation );
            }

            QString newPosterFileName = postersDir + "/" + f.GetPosterName();
            std::string format = settings->GetPosterSavingFormat().toStdString();
            int quality = settings->GetPosterSavingQuality();

            if( !p.save( newPosterFileName, format.c_str(), quality ) )
            {
                f.SetIsPosterExists( false );
                emit PosterMovingError();
            }
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
    eTags->clear();
    cIsViewed->setChecked( false );
    cIsFavourite->setChecked( false );
    eBudget->clear();
    eScreenwriter->clear();
    eComposer->clear();
}
