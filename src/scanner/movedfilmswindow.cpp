/*************************************************************************************************
 *                                                                                                *
 *  file: movedfilmswindow.cpp                                                                    *
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

#include "movedfilmswindow.h"
#include "tools/debug.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>


MovedFilmsWindow::MovedFilmsWindow( QWidget* parent ) : QDialog( parent ),
    filmScannerWorker( new FilmScannerWorker() )
{
    DebugPrintFunc( "MovedFilmsWindow::MovedFilmsWindow" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    eDirectory->setText( QDir::homePath() );
    progressBar->hide();

      // Worker
    connect( filmScannerWorker, &FilmScannerWorker::Scanned, this, &MovedFilmsWindow::ShowFounded );

      // Buttons and view
    connect( bSelectDirectory, &QPushButton::clicked, this, &MovedFilmsWindow::SelectDirectory );
    connect( bScan, &QPushButton::clicked, this, &MovedFilmsWindow::Scan );
    connect( bMove, &QPushButton::clicked, this, &MovedFilmsWindow::MoveSelected );

    connect( gbFounded, &FoundedListWidget::ItemsCountChanged, this, [this] (int count)
    {
        lTotalFounded->setText( QString::number(count) );
    } );

    connect( gbFounded, &FoundedListWidget::SelectionChanged, this, [this] (int count)
    {
        lSelected->setText( QString::number(count) );
    } );

    DebugPrintFuncDone( "MovedFilmsWindow::MovedFilmsWindow" );
}


MovedFilmsWindow::~MovedFilmsWindow()
{
    DebugPrintFunc( "MovedFilmsWindow::~MovedFilmsWindow" );

    if( filmScannerWorker->isRunning() )
    {
        filmScannerWorker->Terminate();
        filmScannerWorker->wait();
    }

    delete filmScannerWorker;

    DebugPrintFuncDone( "MovedFilmsWindow::~MovedFilmsWindow" );
}


void MovedFilmsWindow::SelectDirectory()
{
    QString directory = QFileDialog::getExistingDirectory( this, tr( "Select directory for scanning" ), eDirectory->text() );

    if( !directory.isEmpty() )
    {
        eDirectory->setText( directory );
    }
}


void MovedFilmsWindow::Scan()
{
    DebugPrintFunc( "MovedFilmsWindow::Scan" );

      // If scans
    if( filmScannerWorker->isRunning() )
    {
        filmScannerWorker->Cancel();
        return;
    }

    gbFounded->Clear();

      // Messages
    if( eDirectory->text().isEmpty() )
    {
        QMessageBox::information( this, tr( "Moved films" ), tr( "First select the directory to scan." ) );
        return;
    }

      // Scan
    filmScannerWorker->SetIsRecursive( cSearchInSubdirs->isChecked() );
    filmScannerWorker->SetDir( eDirectory->text() );
    filmScannerWorker->start();

      // Flip button
    bScan->setText( tr( "Cancel" ) );
    progressBar->show();
}


void MovedFilmsWindow::ShowFounded( QStringList fileNames )
{
    DebugPrintFunc( "MovedFilmsWindow::ShowFounded", fileNames.size() );

      // Flip button
    bScan->setText( tr( "Scan" ) );
    progressBar->hide();

      // Show
    for( FilmItem* film : unavailableFilms )
    {
        QString unavailFilePath = film->GetFileName();
        QString unavailFileName = QFileInfo(unavailFilePath).fileName();

        for( QString newFilePath : fileNames )
        {
            QString newFileName = QFileInfo( newFilePath ).fileName();

            if( newFileName == unavailFileName && newFilePath != unavailFilePath ) // Protection from multiple moving
            {
                QTableWidgetItem* item = new QTableWidgetItem();
                item->setData( Qt::DisplayRole, newFilePath );
                item->setData( Qt::UserRole, qVariantFromValue( (void*)film ) );
                gbFounded->AppendItem( item );

                fileNames.removeOne( newFilePath );
                break;
            }
        }
    }

    if( gbFounded->GetItemsCount() == 0 )
    {
        QMessageBox::information( this, tr( "Moved films" ), tr( "Nothing was found." ) );
    }

    DebugPrintFuncDone( "MovedFilmsWindow::ShowFounded" );
}


void MovedFilmsWindow::MoveSelected()
{
    if( lSelected->text().toInt() == 0 )
    {
        QMessageBox::information( this, tr( "Moved films" ), tr( "First select the files to move." ) );
    }

    for( QTableWidgetItem* item : gbFounded->GetItems() )
    {
        if( item->checkState() == Qt::Checked )
        {
              // Uncheck and disable
            item->setCheckState( Qt::Unchecked );
            item->setFlags( Qt::NoItemFlags );
            item->setBackgroundColor( existedFileColor );

              // Change filename
            FilmItem* film = (FilmItem*) item->data( Qt::UserRole ).value<void*>();
            film->SetColumnData( FilmItem::FileNameColumn, item->text() );
            film->SetIsFileExists( FilmItem::Exists );

            gbFounded->ScrollToItem( item );
            unavailableFilms.removeOne( film );
        }
    }

    lSelected->setText( "0" );
    emit FilmsMoved();
}

