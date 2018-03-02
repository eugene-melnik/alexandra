/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerwindow.cpp                                                                   *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

#include "filmscannerwindow.h"
#include "parsers/parsermanager.h"
#include "tools/filesextensions.h"
#include "tools/debug.h"

#include <QFileDialog>
#include <QMessageBox>


FilmScannerWindow::FilmScannerWindow( QWidget* parent ) : QDialog( parent ),
    settings( AlexandraSettings::GetInstance() ),
    filmScannerWorker( new FilmScannerWorker() ),
    filmAddWorker( new FilmScannerAddWorker() )
{
    DebugPrintFunc( "FilmScannerWindow::FilmScannerWindow" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    eDirectory->setText( settings->GetFilmsScannerLastDir() );
    progressBar->hide();

      // Scanner worker
    connect( filmScannerWorker, &FilmScannerWorker::Scanned,         this, &FilmScannerWindow::ShowFound );
    connect( filmScannerWorker, &FilmScannerWorker::IncFoundTotal, this, &FilmScannerWindow::IncFoundTotal );

      // Add worker
    connect( filmAddWorker, &FilmScannerAddWorker::FilmCreated, this, &FilmScannerWindow::AddFilm );
    connect( filmAddWorker, &FilmScannerAddWorker::FilmCreated, this, &FilmScannerWindow::DisableFilm );

      // Parsers
    cbSource->addItems( ParserManager().GetAvailableParsers() );
    cbSource->setCurrentIndex( settings->GetDefaultParserIndex() );
    cDownloadBigPoster->setChecked( settings->GetParsersLoadBigPoster() );
    cDownloadMoreInformation->setChecked( settings->GetParsersLoadAdvancedInfo() );

      // Buttons and view
    connect( bSelectDirectory, &QPushButton::clicked, this, &FilmScannerWindow::SelectDirectory );
    connect( bScan,            &QPushButton::clicked, this, &FilmScannerWindow::Scan );
    connect( bAdd,             &QPushButton::clicked, this, &FilmScannerWindow::AddSelected );

    connect( this->gbFound, &FoundListWidget::ItemsCountChanged, this, [this] (int count)
    {
        this->lTotalFound->setText( QString::number(count) );
    } );

    connect( this->gbFound, &FoundListWidget::SelectionChanged, this, [this] (int count)
    {
        lSelected->setText( QString::number(count) );
    } );

    DebugPrintFuncDone( "FilmScannerWindow::FilmScannerWindow()" );
}


FilmScannerWindow::~FilmScannerWindow()
{
    DebugPrintFunc( "FilmScannerWindow::~FilmScannerWindow" );

    if( filmScannerWorker->isRunning() )
    {
        filmScannerWorker->Terminate();
        filmScannerWorker->wait();
    }
    else if( filmAddWorker->isRunning() )
    {
        filmAddWorker->Cancel();
        filmAddWorker->wait();
    }

    delete filmScannerWorker;
    delete filmAddWorker;

    DebugPrintFuncDone( "FilmScannerWindow::~FilmScannerWindow" );
}


void FilmScannerWindow::SelectDirectory()
{
    QString directory = QFileDialog::getExistingDirectory( this, tr("Select directory for scanning"), eDirectory->text() );

    if( !directory.isEmpty() )
    {
        eDirectory->setText( directory );
    }
}


void FilmScannerWindow::Scan()
{
    DebugPrintFunc( "FilmScannerWindow::Scan" );

      // If scans
    if( filmScannerWorker->isRunning() )
    {
        filmScannerWorker->Cancel();
        return;
    }

    this->gbFound->Clear();

      // Messages
    if( eDirectory->text().isEmpty() )
    {
        QMessageBox::information( this, tr("Film scanner"), tr("Select the directory to scan.") );
        return;
    }

    settings->SetFilmsScannerLastDir( eDirectory->text() );

      // Scan
    filmScannerWorker->SetIsRecursive( cSearchInSubdirs->isChecked() );
    filmScannerWorker->SetDir( eDirectory->text() );
    filmScannerWorker->start();

      // Flip button
    bScan->setEnabled( true );
    bScan->setText( tr("Cancel") );
    progressBar->show();

    DebugPrintFuncDone( "FilmScannerWindow::Scan" );
}


void FilmScannerWindow::AddSelected()
{
    QStringList selectedFilms = this->gbFound->GetSelectedItems();

    if( selectedFilms.empty() ) // Nothing was selected
    {
        QMessageBox::information( this, tr("Film scanner"), tr("Select the files to add.") );
    }
    else
    {
        newFilmsCount = selectedFilms.size();
        progressBar->setMaximum( newFilmsCount );
        progressBar->setValue( 1 );

        bAdd->setEnabled( false );
        progressBar->show();

          // Save parser settings
        settings->SetDefaultParserIndex( cbSource->currentIndex() );
        settings->SetParsersLoadBigPoster( cDownloadBigPoster->isChecked() );
        settings->SetParsersLoadAdvancedInfo( cDownloadMoreInformation->isChecked() );
        settings->sync();

          // Adding worker
        filmAddWorker->SetFoundFilms( selectedFilms );
        filmAddWorker->SetLoadInformation( cLoadInformation->isChecked() );
        filmAddWorker->SetSearchForPoster( cSearchForPoster->isChecked() );
        filmAddWorker->start();
    }
}


void FilmScannerWindow::IncFoundTotal()
{
    int n = lTotalFound->text().toInt() + 1;
    lTotalFound->setText( QString::number( n ) );
}


void FilmScannerWindow::ShowFound( QStringList fileNames )
{
    DebugPrintFunc( "FilmScannerWindow::ShowFound", fileNames.size() );

    bScan->setText( tr("Scan") );
    progressBar->hide();

    if( fileNames.isEmpty() )
    {
        QMessageBox::information( this, tr("Film scanner"), tr("Nothing was found.") );
        return;
    }

    for( const QString& fileName : fileNames )
    {
        #ifdef PORTABLE_VERSION
            bool setDisabled = existedFileNames.contains( QDir(qApp->applicationDirPath()).relativeFilePath(fileName) );
        #else
            bool setDisabled = existedFileNames.contains( fileName );
        #endif

        gbFound->AddItem( fileName, setDisabled );
    }

    DebugPrintFuncDone( "FilmScannerWindow::ShowFound" );
}


void FilmScannerWindow::DisableFilm( FilmItem* film )
{
    QString filmFilename = film->GetFileName();
    existedFileNames.append( filmFilename );

    #ifdef PORTABLE_VERSION
        this->gbFound->DisableItem( QFileInfo(filmFilename).absoluteFilePath(), true );
    #else
        this->gbFound->DisableItem( filmFilename, true );
    #endif

    lSelected->setText( QString::number(--newFilmsCount) );
    progressBar->setValue( progressBar->maximum() - newFilmsCount );

    if( newFilmsCount == 0 )
    {
        bAdd->setEnabled( true );
        progressBar->hide();
    }
}

