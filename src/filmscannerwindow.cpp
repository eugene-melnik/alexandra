
#include "filmscannerwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>

FilmScannerWindow::FilmScannerWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    connect( bSelectDirectory, SIGNAL( clicked() ), this, SLOT( SelectDirectory() ) );
    connect( bScan, SIGNAL( clicked() ), this, SLOT( Scan() ) );

    connect( twFounded, SIGNAL( clicked(QModelIndex) ), this, SLOT( CalculateSelected() ) );

    connect( bSelectAll, SIGNAL( clicked() ), this, SLOT( SelectAll() ) );
    connect( bSelectAll, SIGNAL( clicked() ), this, SLOT( CalculateSelected() ) );
    connect( bUnselectAll, SIGNAL( clicked() ), this, SLOT( UnselectAll() ) );
    connect( bUnselectAll, SIGNAL( clicked() ), this, SLOT( CalculateSelected() ) );
    connect( bInvertSelection, SIGNAL( clicked() ), this, SLOT( InvertSelection() ) );
    connect( bInvertSelection, SIGNAL( clicked() ), this, SLOT( CalculateSelected() ) );

    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( AddSelected() ) );
}

void FilmScannerWindow::show( QStringList* l )
{
    QDialog::show();
    existsFileNames = l;
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

            Film f;
            f.SetFileName( fileName );
            f.SetTitle( QFileInfo( fileName ).fileName() );

            newFilms->append( f );
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
                item->setFlags( Qt::NoItemFlags );
                item->setBackgroundColor( QColor( 0, 255, 0, 40 ) );
            }
        }
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
