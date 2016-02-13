/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerwindow.cpp                                                                   *
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

#include "filmscannerwindow.h"
#include "parsers/parsermanager.h"
#include "tools/filesextensions.h"
#include "tools/debug.h"

#include <string>
#include <QFileDialog>
#include <QMessageBox>


FilmScannerWindow::FilmScannerWindow( QWidget* parent ) : QDialog( parent ),
    settings( AlexandraSettings::GetInstance() ),
    filmScannerWorker( new FilmScannerWorker() ),
    filmAddWorker( new FilmScannerAddWorker() )
{
    DebugPrint( "FilmScannerWindow::FilmScannerWindow()" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    eDirectory->setText( settings->GetFilmsScannerLastDir() );
    progressBar->hide();

      // Scanner worker
    connect( filmScannerWorker, &FilmScannerWorker::Scanned,         this, &FilmScannerWindow::ShowFounded );
    connect( filmScannerWorker, &FilmScannerWorker::IncFoundedTotal, this, &FilmScannerWindow::IncFoundedTotal );

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

    connect( gbFounded, &FoundedListWidget::SelectionChanged, this, [this] (int count)
    {
        lTotalFounded->setText( QString::number(count) );
    } );
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
    QString directory = QFileDialog::getExistingDirectory( this, tr( "Select directory for scanning" ), eDirectory->text() );

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

    gbFounded->Clear();
    lTotalFounded->setText( "0" );

      // Messages
    if( eDirectory->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Film scanner" ), tr( "First select the directory to scan." ) );
        return;
    }

    settings->SetFilmsScannerLastDir( eDirectory->text() );

      // Scan
    filmScannerWorker->SetIsRecursive( cSearchInSubdirs->isChecked() );
    filmScannerWorker->SetDir( eDirectory->text() );
    filmScannerWorker->start();

      // Flip button
    bScan->setEnabled( true );
    bScan->setText( tr( "Cancel" ) );
    progressBar->show();

    DebugPrintFuncDone( "FilmScannerWindow::Scan" );
}


void FilmScannerWindow::AddSelected()
{
    QStringList selectedFilms;

    for( QTableWidgetItem* item : gbFounded->GetItems() )
    {
        if( item->checkState() == Qt::Checked )
        {
            selectedFilms.append( item->text() );
        }
    }

    if( selectedFilms.empty() ) // Nothing was selected
    {
        QMessageBox::information( this, tr( "Film scanner" ), tr( "First select the files to add." ) );
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
        filmAddWorker->SetFoundedFilms( selectedFilms );
        filmAddWorker->SetLoadInformation( cLoadInformation->isChecked() );
        filmAddWorker->SetSearchForPoster( cSearchForPoster->isChecked() );
        filmAddWorker->start();
    }
}


void FilmScannerWindow::IncFoundedTotal()
{
    int n = lTotalFounded->text().toInt() + 1;
    lTotalFounded->setText( QString::number( n ) );
}


void FilmScannerWindow::ShowFounded( QStringList fileNames )
{
    DebugPrintFunc( "FilmScannerWindow::ShowFounded", fileNames.size() );

    bScan->setText( tr( "Scan" ) );
    progressBar->hide();

    if( fileNames.isEmpty() )
    {
        QMessageBox::information( this, tr( "Film scanner" ), tr( "Nothing was found." ) );
        return;
    }

      // Show
    lTotalFounded->setText( QString::number( fileNames.count() ) );

    for( const QString& fileName : fileNames )
    {
        QTableWidgetItem* item = new QTableWidgetItem( fileName );

        if( existedFileNames.contains( fileName ) )
        {
            item->setBackgroundColor( existedFileColor );
            item->setFlags( Qt::NoItemFlags );
        }

        gbFounded->AppendItem( item );
    }

    DebugPrintFuncDone( "FilmScannerWindow::ShowFounded" );
}


void FilmScannerWindow::DisableFilm( FilmItem* film )
{
    QString fileName = film->GetColumnData( FilmItem::FileNameColumn ).toString();

    for( QTableWidgetItem* item : gbFounded->GetItems() )
    {
        if( item->checkState() == Qt::Checked && item->text() == fileName )
        {
              // Uncheck and disable
            item->setCheckState( Qt::Unchecked );
            item->setFlags( Qt::NoItemFlags );
            item->setBackgroundColor( existedFileColor );

            existedFileNames.append( item->text() );
            gbFounded->ScrollToItem( item );
            break;
        }
    }

    lSelected->setText( QString::number(--newFilmsCount) );
    progressBar->setValue( progressBar->maximum() - newFilmsCount );

    if( newFilmsCount == 0 )
    {
        bAdd->setEnabled( true );
        progressBar->hide();
    }
}

