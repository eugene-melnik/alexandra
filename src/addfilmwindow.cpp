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
#include "tools/filesextensions.h"
#include "tools/debug.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <string>

AddFilmWindow::AddFilmWindow( QWidget* parent ) : QDialog( parent )
{
    settings = AlexandraSettings::GetInstance();

    setupUi( this );
    progressBar->hide();
    bOpenFile->setFocus();

    parser = new ParserManager();
    cbOnlineSource->addItems( parser->GetAvailableParsers() );

    connect( parser, &ParserManager::Loaded, this, &AddFilmWindow::InformationLoaded );
    connect( parser, &ParserManager::Error, this, &AddFilmWindow::InformationLoadError );
    connect( parser, &ParserManager::Progress, this, [this] (quint64 value, quint64 maximum) {
        progressBar->setMaximum( maximum );
        progressBar->setValue( value );
    } );

    connect( bOpenFile, &QPushButton::clicked, this, &AddFilmWindow::OpenFilm );
    connect( bOpenPoster, &QPushButton::clicked, this, &AddFilmWindow::OpenPosterFileClicked );
    connect( bLoad, &QPushButton::clicked, this, &AddFilmWindow::LoadInformation );
    connect( bOk, &QPushButton::clicked, this, &AddFilmWindow::OkClicked );

    // TODO: explain this
    QString msgOmdbNotFound = tr( "Movie not found!" );
    Q_UNUSED( msgOmdbNotFound )
}

void AddFilmWindow::show()
{
    DebugPrintFunc( "AddFilmWindow::show" );

    progressBar->hide();
    bOpenPoster->setText( tr( "Open" ) );
    cbOnlineSource->setCurrentIndex( settings->GetDefaultParserIndex() );
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
    DebugPrintFunc( "AddFilmWindow::OpenFilm" );

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
        // If a file is selected set the file name, title and the year (if presents
        // in file name) of the film (with the replacement of characters '_' to spaces)
        eFilmFileName->setText( fileName.absoluteFilePath() );
        QString title = fileName.completeBaseName();

        if( eYear->text().length() < 4 )
        {
            QRegExp regexp( "(185[0-9]|18[6-9][0-9]|19[0-9]{2}|200[0-9]|201[0-9])" ); // Years between 1850 and 2019
            regexp.indexIn( title );
            eYear->setText( regexp.cap(1) );
        }

        if( eTitle->text().isEmpty() )
        {
            eTitle->setText( Film::ClearTitle( title ) );
        }

        // Setting the path to the image file, if found in the same directory
        QString posterFileName = FilesExtensions().SearchForEponymousImage( fileName.absoluteFilePath() );

        if( !posterFileName.isEmpty() )
        {
            ePosterFileName->setText( posterFileName );
            bOpenPoster->setText( tr( "Clear" ) );
        }

        settings->SetLastFilmPath( fileName.absolutePath() );
        bOpenPoster->setFocus();
    }
}

void AddFilmWindow::OpenPosterFileClicked()
{
    DebugPrintFunc( "AddFilmWindow::OpenPosterFileClicked" );

    if( ePosterFileName->text().isEmpty() ) // If poster isn't selected
    {
        DebugPrint( "Select new poster" );

        QFileInfo fileName = QFileDialog::getOpenFileName( this,
                             tr( "Select image" ),
                             settings->GetLastPosterPath(),
                             tr( "Images (%1)" ).arg( FilesExtensions().GetImageExtensionsForFilter() ) );

        if( fileName.isFile() )
        {
            ePosterFileName->setText( fileName.absoluteFilePath() );
            bOpenPoster->setText( tr( "Clear" ) );

            settings->SetLastPosterPath( fileName.absolutePath() );
            eTitle->setFocus();
        }
    }
    else // If poster is already selected
    {
        DebugPrint( "Clearing poster" );

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
                if( !CanBeSaved() )
                {
                    return;
                }

                DebugPrint( "With removing file: " + ePosterFileName->text() );
                QFile( ePosterFileName->text() ).remove();

                // Need to update information if poster removed
                ePosterFileName->clear();
                Save();
            }
        }

        ePosterFileName->clear();
        bOpenPoster->setText( tr( "Open" ) );
    }
}

void AddFilmWindow::LoadInformation()
{
    if( eTitle->text().isEmpty() && eOriginalTitle->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Loading information" ),
                                        tr( "Input title for searching!" ) );
        eTitle->setFocus();
        return;
    }

    QString title = eTitle->text().isEmpty() ? eOriginalTitle->text() : eTitle->text();

    parser->Reset();
    parser->SetParserId( ParserManager::Parser( cbOnlineSource->currentIndex() ) );
    parser->SetTitle( title );

    if( !eYear->text().isEmpty() && eYear->text().length() == 4 )
    {
        parser->SetYear( eYear->text() );
    }

    parser->SetLoadPoster( ePosterFileName->text().isEmpty() );
    parser->Search();

    bLoad->setEnabled( false );
    progressBar->show();
    eTitle->setFocus();
}

void AddFilmWindow::OkClicked()
{
    if( CanBeSaved() )
    {
        Save();
        close();
    }
}

void AddFilmWindow::Save()
{
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
            if( !QFile::exists( postersDir ) )
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
            QString format = settings->GetPosterSavingFormat();
            int quality = settings->GetPosterSavingQuality();

            if( !p.save( newPosterFileName, format.toUtf8(), quality ) )
            {
                f.SetIsPosterExists( false );
                emit PosterMovingError();
            }
        }
    }

    emit Done( f );
}

void AddFilmWindow::InformationLoaded( const Film& f, const QString& posterFileName )
{
    if( !posterFileName.isEmpty() && ePosterFileName->text().isEmpty() )
    {
        ePosterFileName->setText( posterFileName );
        bOpenPoster->setText( tr( "Clear" ) );
    }

    if( eTitle->text().isEmpty() )
    {
        eTitle->setText( f.GetTitle() );
    }

    eOriginalTitle->setText( f.GetOriginalTitle() );
    eTagline->setText( f.GetTagline() );
    eYear->setText( f.GetYearStr() );
    eCountry->setText( f.GetCountry() );
    eGenre->setText( f.GetGenre() );
    cbRating->setCurrentIndex( f.GetRating() - 1 );
    eDirector->setText( f.GetDirector() );
    eProducer->setText( f.GetProducer() );
    tStarring->setPlainText( f.GetStarring() );
    tDescription->setPlainText( f.GetDescription() );
    eBudget->setText( f.GetBudgetStr() );
    eScreenwriter->setText( f.GetScreenwriter() );
    eComposer->setText( f.GetComposer() );

    bLoad->setEnabled( true );
    progressBar->hide();
}

void AddFilmWindow::InformationLoadError( const QString& e )
{
    bLoad->setEnabled( true );
    progressBar->hide();

    QMessageBox::warning( this, tr( "Loading information" ),
                                tr( "Error!\n%1" ).arg( tr( e.toUtf8() ) ) );
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

bool AddFilmWindow::CanBeSaved()
{
    // Checking for necessary fields

    if( eTitle->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Adding film" ), tr( "Field \"Title\" can't be empty." ) );
        eTitle->setFocus();
        return( false );
    }
    else
    {
        return( true );
    }
}
