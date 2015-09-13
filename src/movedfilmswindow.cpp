/*************************************************************************************************
 *                                                                                                *
 *  file: movedfilmswindow.cpp                                                                    *
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

#include "movedfilmswindow.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTableWidgetItem>

MovedFilmsWindow::MovedFilmsWindow( AlexandraSettings* s, QWidget* parent )
    : QDialog( parent ), settings( s ), filmScannerWorker( new FilmScannerWorker() )
{
    setupUi( this );

    // Worker
    connect( filmScannerWorker, &FilmScannerWorker::Scanned, this, &MovedFilmsWindow::ShowFounded );

    // Buttons
    connect( bSelectDirectory, &QPushButton::clicked, this, &MovedFilmsWindow::SelectDirectory );
    connect( bScan, &QPushButton::clicked, this, &MovedFilmsWindow::Scan );

    connect( bSelectAll, &QPushButton::clicked, this, &MovedFilmsWindow::SelectAll );
    connect( bSelectAll, &QPushButton::clicked, this, &MovedFilmsWindow::CalculateSelected );
    connect( bUnselectAll, &QPushButton::clicked, this, &MovedFilmsWindow::UnselectAll );
    connect( bUnselectAll, &QPushButton::clicked, this, &MovedFilmsWindow::CalculateSelected );
    connect( bInvertSelection, &QPushButton::clicked, this, &MovedFilmsWindow::InvertSelection );
    connect( bInvertSelection, &QPushButton::clicked, this, &MovedFilmsWindow::CalculateSelected );

    connect( bMove, &QPushButton::clicked, this, &MovedFilmsWindow::MoveSelected );
    connect( twFounded, &QTableWidget::clicked, this, &MovedFilmsWindow::CalculateSelected );
}

MovedFilmsWindow::~MovedFilmsWindow()
{
    delete filmScannerWorker;
}

void MovedFilmsWindow::show( QList<Film*>* f )
{
    films = f;

    if( films->size() == 0 )
    {
        QMessageBox::information( dynamic_cast<QWidget*>( parent() ),
                                  tr( "Moved films" ),
                                  tr( "Nothing to move." ) );
    }
    else
    {
        progressBar->hide();
        QDialog::show();
    }
}

void MovedFilmsWindow::closeEvent( QCloseEvent* event )
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
    delete films;

    event->accept();
}

void MovedFilmsWindow::SelectDirectory()
{
    QString directory = QFileDialog::getExistingDirectory( this,
                                                           tr( "Select directory for scanning" ),
                                                           eDirectory->text() );

    if( !directory.isEmpty() )
    {
        eDirectory->setText( directory );
    }
}

void MovedFilmsWindow::Scan()
{
    // If scans
    if( bScan->text() == tr( "Cancel" ) )
    {
        filmScannerWorker->Cancel();
        return;
    }

    twFounded->clearContents();
    twFounded->setRowCount( 0 );

    // Messages
    if( eDirectory->text().isEmpty() )
    {
        QMessageBox::information( this,
                                  tr( "Moved films" ),
                                  tr( "First select the directory to scan." ) );
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

void MovedFilmsWindow::ShowFounded(QList<QString> *fileNames)
{
    // Flip button
    bScan->setText( tr( "Scan" ) );
    progressBar->hide();

    // Messages
    if( fileNames->empty() )
    {
        QMessageBox::information( this,
                                  tr( "Moved films" ),
                                  tr( "Nothing was found." ) );
        return;
    }

    // Show
    int row = 0;

    for( QList<QString>::iterator i = fileNames->begin(); i < fileNames->end(); i++ )
    {
        QString newFileNameFull = *i;

        QTableWidgetItem* item = new QTableWidgetItem( newFileNameFull );
        item->setCheckState( Qt::Unchecked );

        QString newFileName = QFileInfo( newFileNameFull ).fileName();

        for( int i = 0; i < films->size(); i++ )
        {
            QString unavailFileNameFull = films->at(i)->GetFileName();
            QString unavailFileName = QFileInfo( unavailFileNameFull ).fileName();

            if( newFileName == unavailFileName
                    && newFileNameFull != unavailFileNameFull ) // Protection from multiple moving
            {
                twFounded->setRowCount( twFounded->rowCount() + 1 );
                twFounded->setItem( row++, 0, item );
                break;
            }
        }
    }

    lTotalFounded->setText( QString::number( row ) );
    delete fileNames;
}

void MovedFilmsWindow::SelectAll()
{
    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );
        item->setCheckState( Qt::Checked );
    }
}

void MovedFilmsWindow::UnselectAll()
{
    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );
        item->setCheckState( Qt::Unchecked );
    }
}

void MovedFilmsWindow::InvertSelection()
{
    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

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

void MovedFilmsWindow::CalculateSelected()
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

void MovedFilmsWindow::MoveSelected()
{
    QList<QString> newPathes;

    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->checkState() == Qt::Checked )
        {
            // Saving new pathes
            newPathes.append( twFounded->item( i, 0 )->text() );
            // Removing checked rows
            twFounded->removeRow( i-- );
        }
    }

    if( newPathes.empty() )
    {
        QMessageBox::information( this,
                                  tr( "Moved films" ),
                                  tr( "First select the files to move." ) );
    }
    else
    {
        for( int i = 0; i < newPathes.size(); i++ )
        {
            QString newFileName = QFileInfo( newPathes.at( i ) ).fileName();

            for( int j = 0; j < films->size(); j++ )
            {
                QString oldFileName = QFileInfo( films->at( j )->GetFileName() ).fileName();

                if( newFileName == oldFileName )
                {
                    // Setting new file path
                    films->at( j )->SetFileName( newPathes.at( i ) );
                    break;
                }
            }
        }

        emit FilmsMoved();
    }

    lTotalFounded->setText( QString::number( twFounded->rowCount() ) );
    lSelected->setText( "0" );
}
