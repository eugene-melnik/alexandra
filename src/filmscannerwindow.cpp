/*************************************************************************************************
 *                                                                                                *
 *  file: filmscannerwindow.cpp                                                                   *
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

#include "filmscannerwindow.h"
#include "filesextensions.h"

#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

FilmScannerWindow::FilmScannerWindow( AlexandraSettings* s, QWidget* parent )
    : QDialog( parent ), settings( s ), newFilms( new QList<Film>() )
{
    setupUi( this );

    // Scanner worker
    filmScannerWorker = new FilmScannerWorker();
    connect( filmScannerWorker, &FilmScannerWorker::Scanned, this, &FilmScannerWindow::ShowFounded );
    connect( filmScannerWorker, &FilmScannerWorker::IncFoundedTotal, this, &FilmScannerWindow::IncFoundedTotal );

    // Buttons
    connect( bSelectDirectory, &QPushButton::clicked, this, &FilmScannerWindow::SelectDirectory );
    connect( bScan, &QPushButton::clicked, this, &FilmScannerWindow::Scan );
    connect( bAdd, &QPushButton::clicked, this, &FilmScannerWindow::AddSelected );

    connect( bSelectAll, &QPushButton::clicked, this, &FilmScannerWindow::SelectAll );
    connect( bSelectAll, &QPushButton::clicked, this, &FilmScannerWindow::CalculateSelected );
    connect( bUnselectAll, &QPushButton::clicked, this, &FilmScannerWindow::UnselectAll );
    connect( bUnselectAll, &QPushButton::clicked, this, &FilmScannerWindow::CalculateSelected );
    connect( bInvertSelection, &QPushButton::clicked, this, &FilmScannerWindow::InvertSelection );
    connect( bInvertSelection, &QPushButton::clicked, this, &FilmScannerWindow::CalculateSelected );

    connect( twFounded, &QTableWidget::clicked, this, &FilmScannerWindow::CalculateSelected );
}

FilmScannerWindow::~FilmScannerWindow()
{
    delete newFilms;
    delete filmScannerWorker;
}

void FilmScannerWindow::show( QStringList* l )
{
    existsFileNames = l;
    progressBar->hide();
    eDirectory->setText( settings->GetFilmsScannerLastDir() );

    QDialog::show();
}

void FilmScannerWindow::closeEvent( QCloseEvent* event )
{
    // Stop worker if scans
    if( bScan->text() == tr( "Cancel" ) )
    {
        filmScannerWorker->Terminate();
        bScan->setText( tr( "Scan" ) );
    }

    // Clear contents
    eDirectory->clear();
    cSearchInSubdirs->setChecked( true );
    twFounded->clearContents();
    twFounded->setRowCount( 0 );
    lTotalFounded->setText( "0" );
    lSelected->setText( "0" );
    // Temporary data
    delete existsFileNames;

    event->accept();
}

void FilmScannerWindow::SelectDirectory()
{
    QString directory = QFileDialog::getExistingDirectory( this,
                                                           tr( "Select directory for scanning" ),
                                                           eDirectory->text() );

    if( !directory.isEmpty() )
    {
        eDirectory->setText( directory );
    }
}

void FilmScannerWindow::Scan()
{
    // If scans
    if( bScan->text() == tr( "Cancel" ) )
    {
        filmScannerWorker->Cancel();
        return;
    }

    twFounded->clearContents();
    twFounded->setRowCount( 0 );
    lTotalFounded->setText( "0" );

    // Messages
    if( eDirectory->text().isEmpty() )
    {
        QMessageBox::information( this,
                                  tr( "Film scanner" ),
                                  tr( "First select the directory to scan." ) );
        return;
    }

    settings->SetFilmsScannerLastDir( eDirectory->text() );

    // Scan
    filmScannerWorker->SetIsRecursive( cSearchInSubdirs->isChecked() );
    filmScannerWorker->SetDir( eDirectory->text() );
    filmScannerWorker->start();

    // Flip button
    bScan->setText( tr( "Cancel" ) );
    progressBar->show();
}

void FilmScannerWindow::IncFoundedTotal()
{
    bool isOk;
    int n = lTotalFounded->text().toInt( &isOk ) + 1;
    if( isOk ) lTotalFounded->setText( QString::number( n ) );
}

void FilmScannerWindow::ShowFounded( QList<QString>* fileNames )
{
    // Flip button
    bScan->setText( tr( "Scan" ) );
    progressBar->hide();

    // Messages
    if( fileNames->empty() )
    {
        QMessageBox::information( this,
                                  tr( "Film scanner" ),
                                  tr( "Nothing was found." ) );
        return;
    }

    // Show
    lTotalFounded->setText( QString::number( fileNames->size() ) );
    twFounded->setRowCount( fileNames->size() );
    int row = 0;

    for( QList<QString>::iterator i = fileNames->begin(); i < fileNames->end(); i++ )
    {
        QString fileName = *i;

        QTableWidgetItem* item = new QTableWidgetItem( fileName );
        item->setCheckState( Qt::Unchecked );

        if( existsFileNames->contains( fileName ) )
        {
            item->setBackgroundColor( QColor( 0, 255, 0, 40 ) );
            item->setFlags( Qt::NoItemFlags );
        }

        twFounded->setItem( row++, 0, item );
    }

    delete fileNames;
}

void FilmScannerWindow::SelectAll()
{
    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->flags() != Qt::NoItemFlags )
        {
            item->setCheckState( Qt::Checked );
        }
    }
}

void FilmScannerWindow::UnselectAll()
{
    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->flags() != Qt::NoItemFlags )
        {
            item->setCheckState( Qt::Unchecked );
        }
    }
}

void FilmScannerWindow::InvertSelection()
{
    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->flags() != Qt::NoItemFlags )
        {
            if( item->checkState() == Qt::Unchecked )
            {
                item->setCheckState( Qt::Checked );
            }
            else
            {
                item->setCheckState( Qt::Unchecked );
            }
        }
    }
}

void FilmScannerWindow::CalculateSelected()
{
    int count = 0;

    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->checkState() == Qt::Checked )
        {
            count++;
        }
    }

    lSelected->setText( QString::number( count ) );
}

void FilmScannerWindow::AddSelected()
{
    QStringList selectedFilms;

    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->checkState() == Qt::Checked )
        {
            selectedFilms.append( twFounded->item( i, 0 )->text() );
        }
    }

    if( selectedFilms.empty() ) // Nothing was selected
    {
        QMessageBox::information( this,
                                  tr( "Film scanner" ),
                                  tr( "First select the files to add." ) );
    }
    else
    {
        newFilmsCount = selectedFilms.size();
        bAdd->setEnabled( false );
        progressBar->show();

        // Threads create
        int threadCount = 1; /* QThread::idealThreadCount(); */ // TODO: Problems in QPixmap::save in multithread
        int subListLength = selectedFilms.size() / threadCount;

        for( int threadNum = 0; threadNum < threadCount; threadNum++ )
        {
            int subListPos = threadNum * subListLength;

            if( threadNum == (threadCount - 1) ) // Last thread
            {
                subListLength = -1;
            }

            FilmScannerAddWorker* addWorker = new FilmScannerAddWorker();
            connect( addWorker, &FilmScannerAddWorker::FilmsCreated, this, &FilmScannerWindow::FilmsCreated );
            connect( addWorker, &FilmScannerAddWorker::finished, addWorker, &QWidget::deleteLater );

            addWorker->SetFoundedFilms( selectedFilms.mid( subListPos, subListLength ) );
            addWorker->SetSettings( settings );
            addWorker->SetSearchForPoster( cSearchForPoster->isChecked() );
            addWorker->start();
        }
    }
}

void FilmScannerWindow::FilmsCreated( QList<Film> films )
{
    newFilms->append( films );

    if( newFilms->size() == newFilmsCount )
    {
        // Disabling already added files
        for( int i = 0; i < twFounded->rowCount(); i++ )
        {
            QTableWidgetItem* item = twFounded->item( i, 0 );

            if( item->checkState() == Qt::Checked )
            {
                // Uncheck
                item->setCheckState( Qt::Unchecked );
                item->setFlags( Qt::NoItemFlags );
                item->setBackgroundColor( QColor( 0, 255, 0, 40 ) );
                // Add to list of existing films
                existsFileNames->append( item->text() );
            }
        }

        emit AddFilms( newFilms );

        lSelected->setText( "0" );
        bAdd->setEnabled( true );
        progressBar->hide();
        newFilms->clear();
    }
}
