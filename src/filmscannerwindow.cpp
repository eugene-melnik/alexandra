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

#include <QCryptographicHash>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

FilmScannerWindow::FilmScannerWindow( AlexandraSettings* s, QWidget* parent )
    : QDialog( parent ), settings( s )
{
    setupUi( this );

    connect( bSelectDirectory, &QPushButton::clicked, this, &FilmScannerWindow::SelectDirectory );
    connect( bScan, &QPushButton::clicked, this, &FilmScannerWindow::Scan );

    connect( twFounded, &QTableWidget::clicked, this, &FilmScannerWindow::CalculateSelected );

    connect( bSelectAll, &QPushButton::clicked, this, &FilmScannerWindow::SelectAll );
    connect( bSelectAll, &QPushButton::clicked, this, &FilmScannerWindow::CalculateSelected );
    connect( bUnselectAll, &QPushButton::clicked, this, &FilmScannerWindow::UnselectAll );
    connect( bUnselectAll, &QPushButton::clicked, this, &FilmScannerWindow::CalculateSelected );
    connect( bInvertSelection, &QPushButton::clicked, this, &FilmScannerWindow::InvertSelection );
    connect( bInvertSelection, &QPushButton::clicked, this, &FilmScannerWindow::CalculateSelected );

    connect( bAdd, &QPushButton::clicked, this, &FilmScannerWindow::AddSelected );
}

void FilmScannerWindow::show( QStringList* l )
{
    QDialog::show();
    existsFileNames = l;
    eDirectory->setText( settings->GetFilmsScannerLastDir() );
}

void FilmScannerWindow::closeEvent( QCloseEvent* event )
{
    // Clear contents
    eDirectory->clear();
    cSearchInSubdirs->setChecked( true );
    twFounded->clearContents();
    twFounded->setRowCount( 0 );
    lTotalFounded->setText( "0" );
    lSelected->setText( "0" );

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
    twFounded->clearContents();
    twFounded->setRowCount( 0 );

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
    QString dir = eDirectory->text();
    QList<QString>* fileNames = nullptr;

    if( cSearchInSubdirs->isChecked() )
    {
        fileNames = ScanDirectoryRecursive( dir );
    }
    else
    {
        fileNames = ScanDirectory( dir );
    }

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
    QList<Film>* newFilms = new QList<Film>();

    for( int i = 0; i < twFounded->rowCount(); i++ )
    {
        QTableWidgetItem* item = twFounded->item( i, 0 );

        if( item->checkState() == Qt::Checked )
        {
            QString fileName = twFounded->item( i, 0 )->text();

            // Creating film
            Film f;
            f.SetId( QString( QCryptographicHash::hash( QByteArray::number( qrand() ), QCryptographicHash::Sha1 ).toHex() ) );
            f.SetFileName( fileName );
            f.SetTitle( QFileInfo( fileName ).fileName() );
            newFilms->append( f );

            // Adding to the existing films
            existsFileNames->append( fileName );
        }
    }

    if( newFilms->empty() )
    {
        QMessageBox::information( this,
                                  tr( "Film scanner" ),
                                  tr( "First select the files to add." ) );
    }
    else
    {
        emit AddFilms( newFilms );

        // Disabling already added files
        for( int i = 0; i < twFounded->rowCount(); i++ )
        {
            QTableWidgetItem* item = twFounded->item( i, 0 );

            if( item->checkState() == Qt::Checked )
            {
                item->setCheckState( Qt::Unchecked );
                item->setFlags( Qt::NoItemFlags );
                item->setBackgroundColor( QColor( 0, 255, 0, 40 ) );
            }
        }

        lSelected->setText( "0" );
    }

    delete newFilms;
}

QList<QString>* FilmScannerWindow::ScanDirectory( QString dir )
{
    QList<QString>* result = new QList<QString>();
    QFileInfoList files = QDir( dir ).entryInfoList( filter );

    for( QList<QFileInfo>::iterator i = files.begin(); i < files.end(); i++ )
    {
        result->append( i->absoluteFilePath() );
    }

    return( result );
}

QList<QString>* FilmScannerWindow::ScanDirectoryRecursive( QString dir )
{
    QList<QString>* result = new QList<QString>();

    // Scan subdirectories recursively
    QFileInfoList files = QDir( dir ).entryInfoList();

    for( QList<QFileInfo>::iterator i = files.begin(); i < files.end(); i++ )
    {
        if( i->isDir() && (i->fileName() != ".") && (i->fileName() != "..") )
        {
            QList<QString>* subdirFiles = ScanDirectoryRecursive( i->absoluteFilePath() );
            result->append( *subdirFiles );
            delete subdirFiles;
        }
    }

    // Scan files
    files = QDir( dir ).entryInfoList( filter );

    for( QList<QFileInfo>::iterator i = files.begin(); i < files.end(); i++ )
    {
        result->append( i->absoluteFilePath() );
    }

    return( result );
}
