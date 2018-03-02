/*************************************************************************************************
 *                                                                                                *
 *  file: movedfilmswindow.cpp                                                                    *
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
    connect( filmScannerWorker, &FilmScannerWorker::Scanned, this, &MovedFilmsWindow::ShowFound );

      // Buttons and view
    connect( bSelectDirectory, &QPushButton::clicked, this, &MovedFilmsWindow::SelectDirectory );
    connect( bScan, &QPushButton::clicked, this, &MovedFilmsWindow::Scan );
    connect( bMove, &QPushButton::clicked, this, &MovedFilmsWindow::MoveSelected );

    connect( this->gbFound, &FoundListWidget::ItemsCountChanged, this, [this] (int count)
    {
        this->lTotalFound->setText( QString::number(count) );
    } );

    connect( this->gbFound, &FoundListWidget::SelectionChanged, this, [this] (int count)
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
    QString directory = QFileDialog::getExistingDirectory( this, tr("Select directory for scanning"), eDirectory->text() );

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

    this->gbFound->Clear();

      // Messages
    if( eDirectory->text().isEmpty() )
    {
        QMessageBox::information( this, tr("Moved films"), tr("Select the directory to scan.") );
        return;
    }

      // Scan
    filmScannerWorker->SetIsRecursive( cSearchInSubdirs->isChecked() );
    filmScannerWorker->SetDir( eDirectory->text() );
    filmScannerWorker->start();

      // Flip button
    bScan->setText( tr("Cancel") );
    progressBar->show();
}


void MovedFilmsWindow::ShowFound( QStringList fileNames )
{
    DebugPrintFunc( "MovedFilmsWindow::ShowFound", fileNames.size() );

      // Flip button
    bScan->setText( tr("Scan") );
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
                this->gbFound->AddItem( newFilePath, qVariantFromValue( (void*)film ) );
                fileNames.removeOne( newFilePath );
                break;
            }
        }
    }

    if( this->gbFound->GetItemsCount() == 0 )
    {
        QMessageBox::information( this, tr("Moved films"), tr("Nothing was found.") );
    }

    DebugPrintFuncDone( "MovedFilmsWindow::ShowFound" );
}


void MovedFilmsWindow::MoveSelected()
{
    if( lSelected->text().toInt() == 0 )
    {
        QMessageBox::information( this, tr("Moved films" ), tr("Select the files to move.") );
        return;
    }

    for( QPair<QString,QVariant> itemData : this->gbFound->GetSelectedItemsData() )
    {
          // Uncheck and disable
        this->gbFound->DisableItem( itemData.first, true );

          // Change filename
        FilmItem* film = (FilmItem*) itemData.second.value<void*>();
        film->SetColumnData( FilmItem::FileNameColumn, itemData.first );
        film->SetIsFileExists( FilmItem::Exists );

        unavailableFilms.removeOne( film );
    }

    lSelected->setText( "0" );
    emit FilmsMoved();
}

