/*************************************************************************************************
 *                                                                                                *
 *  file: addfilmwindow.cpp                                                                       *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>                                     *
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
#include "texteditor.h"
#include "tools/filesextensions.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <string>


AddFilmWindow::AddFilmWindow( QWidget* parent ) : QDialog( parent ),
    filmId( FilmItem::GetRandomHash() ),
    settings( AlexandraSettings::GetInstance() )
{
    DebugPrint( "AddFilmWindow::AddFilmWindow()" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );

    bOpenFile->setFocus();
    progressBar->hide();

      // Buttons
    connect( bOpenFile, &QPushButton::clicked, this, &AddFilmWindow::OpenFilm );
    connect( bOpenPoster, &QPushButton::clicked, this, &AddFilmWindow::OpenPosterFileClicked );
    connect( bLoad, &QPushButton::clicked, this, &AddFilmWindow::LoadInformation );
    connect( bClearYear, &QPushButton::clicked, this, [this] { sbYear->setValue(0); } );
    connect( bOk, &QPushButton::clicked, this, &AddFilmWindow::OkClicked );

    connect( bStarringEdit, &QPushButton::clicked, this, &AddFilmWindow::EditStarring );
    connect( bDescriptionEdit, &QPushButton::clicked, this, &AddFilmWindow::EditDescription );

      // Setup parsers
    parser = new ParserManager();
    cbOnlineSource->addItems( parser->GetAvailableParsers() );
    cbOnlineSource->setCurrentIndex( settings->GetDefaultParserIndex() );

    connect( parser, &ParserManager::Loaded, this, &AddFilmWindow::InformationLoaded );
    connect( parser, &ParserManager::Error, this, &AddFilmWindow::InformationLoadError );
    connect( parser, &ParserManager::Progress, this, [this] (quint64 value, quint64 maximum)
    {
        progressBar->setMaximum( maximum );
        progressBar->setValue( value );
    } );
}


AddFilmWindow::~AddFilmWindow()
{
    DebugPrint( "AddFilmWindow::~AddFilmWindow()" );
    delete parser;
}


void AddFilmWindow::OpenFilm()
{
    DebugPrintFunc( "AddFilmWindow::OpenFilm" );

      // Set path to the file depending on the settings
      // or the selected movie (if already selected)
    QString openPath = settings->GetLastFilmPath();

    if( !eFilmFileName->text().isEmpty() )
    {
        openPath = eFilmFileName->text(); /// without fileinfo?
    }

      // Open file dialog
    QFileInfo fileName = QFileDialog::getOpenFileName( this, tr( "Select film" ), openPath,
                                                       tr( "Video files (%1)" ).arg( FilesExtensions().GetFilmExtensionsForFilter() ) );

    if( fileName.isFile() )
    {
          // If a file is selected set the file name, title and the year (if presents
          // in file name) of the film (with the replacement of characters '_' to spaces)
        eFilmFileName->setText( fileName.absoluteFilePath() );
        QString title = fileName.completeBaseName();

        if( eTitle->text().isEmpty() )
        {
            eTitle->setText( FilmItem::ClearTitle( title ) );
        }

        QRegExp regexp( "(185[0-9]|18[6-9][0-9]|19[0-9]{2}|200[0-9]|201[0-9])" ); // Years between 1850 and 2019
        regexp.indexIn( title );

        if( sbYear->value() == sbYear->minimum() )
        {
            sbYear->setValue( regexp.cap(1).toInt() );
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
        DebugPrint( "Selecting new poster" );

        QFileInfo fileName = QFileDialog::getOpenFileName( this, tr( "Select image" ), settings->GetLastPosterPath(),
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
          // it has been moved to the posters directory (for "EditFilm" window)

        QString posterPath = QFileInfo( ePosterFileName->text() ).absolutePath();
        QString postersDirPath = settings->GetPostersDirPath();

        if( posterPath == postersDirPath )
        {
            int res = QMessageBox::question( this, tr( "Clear poster" ), tr( "Remove image file?" ) );

            if( res == QMessageBox::Yes )
            {
                if( !CanBeSaved() ) return;

                DebugPrint( "With removing file: " + ePosterFileName->text() );
                QFile( ePosterFileName->text() ).remove();

                  // Need to update information if poster was removed
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
        QMessageBox::information( this, tr( "Loading information" ), tr( "Input title for searching!" ) );
        eTitle->setFocus();
        return;
    }

    parser->Reset();
    parser->SetParserId( ParserManager::Parser( cbOnlineSource->currentIndex() ) );
    parser->SetTitle( eTitle->text().isEmpty() ? eOriginalTitle->text() : eTitle->text() );

    if( sbYear->value() != sbYear->minimum() )
    {
        parser->SetYear( sbYear->text() );
    }

    parser->SetLoadPoster( ePosterFileName->text().isEmpty() );
    parser->Search();

    bLoad->setEnabled( false );
    bOk->setEnabled( false );
    bCancel->setEnabled( false );
    progressBar->show();
    eTitle->setFocus();
}


void AddFilmWindow::ShowEditor( CutLabel* editDataIn, const QString& title )
{
    TextEditor* editor = new TextEditor( this );
    editor->SetDataSource( editDataIn );
    editor->SetDataReceiver( editDataIn );
    editor->SetTitle( title );
    editor->show();
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
    QList<QVariant> data;

      // Order of the columns in file "filmslist/filmitem.h"
    data << eTitle->text()
         << eOriginalTitle->text()
         << eTagline->text()
         << sbYear->value()
         << eGenre->text()
         << eCountry->text()
         << eDirector->text()
         << eProducer->text()
         << eScreenwriter->text()
         << eComposer->text()
         << sbBudget->value()
         << sbRating->value()
         << cIsViewed->isChecked()
         << cIsFavourite->isChecked()
         << 0 // Views count
         << lStarringText->text()
         << lDescriptionText->text()
         << eTags->toPlainText()
         << eFilmFileName->text()
         << filmId;

    FilmItem* film = new FilmItem( data );
    film->SetFilmId( filmId );

      // Manipulations with poster
    QString posterFileName = ePosterFileName->text();

    if( !posterFileName.isEmpty() )
    {
        QString postersDir = settings->GetPostersDirPath();

          // Create a directory for posters if it does not exist
        if( !QFile::exists( postersDir ) )
        {
            QDir().mkdir( postersDir );
        }

        int newHeight = settings->GetScalePosterToHeight();
        QPixmap pixmap( posterFileName );

          // Move to directory of the posters with selected format and quality
          // Scaling if set in options
        if( newHeight != 0 && newHeight < pixmap.height() )
        {
            pixmap = pixmap.scaledToHeight( newHeight, Qt::SmoothTransformation );
        }

        QString newPosterFileName = postersDir + "/" + film->GetFilmId();

        if( newPosterFileName == posterFileName ) // "EditFilmWindow" mode
        {
            film->SetIsPosterExists( FilmItem::Exists );
        }
        else
        {
            QString newFormat = settings->GetPosterSavingFormat();
            int newQuality = settings->GetPosterSavingQuality();

            if( pixmap.save( newPosterFileName, newFormat.toUtf8(), newQuality ) )
            {
                film->SetIsPosterExists( FilmItem::Exists );
            }
            else
            {
                QMessageBox::warning( this, tr( "Saving film" ), tr( "Error while moving poster to posters directory." ) );
            }
        }
    }

    emit Done( film );
}


void AddFilmWindow::InformationLoaded( const FilmItem& film, const QString& posterFileName )
{
    filmId = film.GetFilmId();

    if( ePosterFileName->text().isEmpty() && !posterFileName.isEmpty() )
    {
        ePosterFileName->setText( posterFileName );
        bOpenPoster->setText( tr( "Clear" ) );
    }

    if( eTitle->text().isEmpty() )
    {
        eTitle->setText( film.GetColumnData( FilmItem::TitleColumn ).toString() );
    }

    eOriginalTitle->setText( film.GetColumnData( FilmItem::OriginalTitleColumn ).toString() );
    eTagline->setText( film.GetColumnData( FilmItem::TaglineColumn ).toString() );

    sbYear->setValue( film.GetColumnData( FilmItem::YearColumn ).toInt() );
    sbBudget->setValue( film.GetColumnData( FilmItem::BudgetColumn ).toDouble() );
    sbRating->setValue( film.GetColumnData( FilmItem::RatingColumn ).toDouble() );

    eCountry->setText( film.GetColumnData( FilmItem::CountryColumn ).toString() );
    eGenre->setText( film.GetColumnData( FilmItem::GenreColumn ).toString() );
    eDirector->setText( film.GetColumnData( FilmItem::DirectorColumn ).toString() );
    eProducer->setText( film.GetColumnData( FilmItem::ProducerColumn ).toString() );
    eScreenwriter->setText( film.GetColumnData( FilmItem::ScreenwriterColumn ).toString() );
    eComposer->setText( film.GetColumnData( FilmItem::ComposerColumn ).toString() );

    lStarringText->setText( film.GetColumnData( FilmItem::StarringColumn ).toString() );

    lDescriptionText->setText( film.GetColumnData( FilmItem::DescriptionColumn ).toString() );

    bLoad->setEnabled( true );
    bOk->setEnabled( true );
    bCancel->setEnabled( true );
    progressBar->hide();
}


void AddFilmWindow::InformationLoadError( const QString& error )
{
    bLoad->setEnabled( true );
    progressBar->hide();
    QMessageBox::warning( this, tr( "Loading information" ), tr( "Error!\n%1" ).arg( tr( error.toUtf8() ) ) );

    if( sbYear->value() != sbYear->minimum() )
    {
        QMessageBox::information( this, tr( "Loading information" ),
                                  tr( "Please note that the search is performed on the fields \"Title\" (or \"Original title\") "
                                      "and \"Year\" (if specified). Perhaps some of these data are incorrect." ) );
    }
}
