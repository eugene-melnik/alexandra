/*************************************************************************************************
 *                                                                                                *
 *  file: settingswindow.cpp                                                                      *
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

#include "settingswindow.h"
#include "version.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QMessageBox>

SettingsWindow::SettingsWindow( AlexandraSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    setupUi( this );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );

    ConfigureApplicationTab();
    ConfigureDatabaseTab();
}

void SettingsWindow::showEvent( QShowEvent* event )
{
    isSettingsChanged = false;
    isNeedReboot = false;

    ReconfigureApplicationTab();
    ReconfigureDatabaseTab();
    event->accept();
}

/*************************************************************************************************
 *  Events' handlers (slots)                                                                      *
  *************************************************************************************************/

// Window

void SettingsWindow::OkButtonClicked()
{
    // Show reboot message (if necessary)
    if( isNeedReboot ) {
        QMessageBox::information( this, tr( "Settings" ), tr( "For taking all settings, restart the application." ) );
    }

    // Hide window
    close();

    // Saving settings
    if( isSettingsChanged ) {
        // application tab
        if( cbStyle->currentIndex() == 0 ) {
            settings->SetApplicationStyle( "" );
        } else {
            settings->SetApplicationStyle( cbStyle->currentText() );
        }

        settings->SetMainWindowToolbarStyle( toolStyles[ cbToolbarStyle->currentIndex() ].style  );
        settings->SetApplicationExternalPlayer( eExternalPlayer->text() );
        // database tab
        settings->SetApplicationDatabaseFile( eDatabaseFile->text() );
        settings->SetFilmsListCheckFilesOnStartup( cCheckFilesAtStartup->isChecked() );
        settings->SetFilmsListPostersDir( ePostersFolder->text() );
        settings->SetFilmsListScalePosters( cScalePoster->isChecked() ? sbScaleToHeight->value() : 0 );
        settings->sync();

        emit SettingsChanged();

        if( isDatabaseSettingsChanged ) {
            emit DatabaseSettingsChanged();
        }
    }
}

void SettingsWindow::SetIsSettingsChanged()
{
    isSettingsChanged = true;
}

void SettingsWindow::SetIsNeedReboot()
{
    SetIsSettingsChanged();
    isNeedReboot = true;
}

void SettingsWindow::SetIsDatabaseSettingsChanged()
{
    SetIsSettingsChanged();
    isDatabaseSettingsChanged = true;
}

void SettingsWindow::SelectExternalPlayer()
{
    QString externalPlayer = QFileDialog::getOpenFileName( this,
                                                           tr( "Select external player" ),
                                                           eExternalPlayer->text(),
#ifdef Q_OS_WIN32
                                                           tr( "Executable files (*.exe)" ) );
#else
                                                           tr( "Executable files (*)" ) );
#endif

    if( !externalPlayer.isEmpty() ) {
        eExternalPlayer->setText( externalPlayer );
    }
}

// Application tab

void SettingsWindow::SetDefaultExternalPlayer()
{
#ifdef Q_OS_LINUX
    eExternalPlayer->setText( "xdg-open" );
#elif defined(Q_OS_WIN32)
    eExternalPlayer->setText( "C:\\Program Files\\Windows Media Player\\wmplayer.exe" );
#else
    eExternalPlayer->clear();
#endif
}

// Database tab

void SettingsWindow::OpenDatabaseFile()
{
    QString databaseFileName = QFileDialog::getOpenFileName( this,
                                                             tr( "Select database file" ),
                                                             eDatabaseFile->text(),
                                                             tr( "Alexandra DB (*.adat)" ) );

    if( !databaseFileName.isEmpty() ) {
        eDatabaseFile->setText( databaseFileName );

        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";

        if( QDir( postersDir ).exists() ) {
            int res = QMessageBox::question( this,
                                             tr( "Settings" ),
                                             tr( "Would you like to set the catalog of posters is \"%1\"?" ).arg( postersDir ),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            if( res == QMessageBox::Yes ) {
                ePostersFolder->setText( postersDir );
            }
        }
    }
}

void SettingsWindow::SelectColorUnavailable()
{
    QColor oldColor = settings->GetFilmsListUnavailableFileColor();
    QColor newColor = QColorDialog::getColor( oldColor, this );

    // If cancel button pressed
    if( newColor.isValid() ) {
        settings->SetFilmsListUnavailableFileColor( newColor.rgba() );
        isDatabaseSettingsChanged = true;
    }
}

void SettingsWindow::CreateDatabase()
{
    QString databaseFileName = QFileDialog::getSaveFileName( this,
                                                             tr( "Create database" ),
                                                             "database.adat",
                                                             tr( "Alexandra DB (*.adat)" ) );

    if( !databaseFileName.isEmpty() ) {
        eDatabaseFile->setText( databaseFileName );

        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";

        if( !QDir( postersDir ).exists() ) {
            int res = QMessageBox::question( this,
                                             tr( "Create database" ),
                                             tr( "Would you like to set the catalog of posters next to the database file?" ),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            if( res == QMessageBox::Yes ) {
                ePostersFolder->setText( postersDir );
            }
        }
    }
}

void SettingsWindow::EraseDatabaseQuestion()
{
    int res = QMessageBox::warning( this,
                                    tr( "Erase database" ),
                                    tr( "Are you sure you want to erase the database and posters?" ),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( res == QMessageBox::Yes ) {
        emit EraseDatabase();
    }
}

void SettingsWindow::OpenPostersFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory( this,
                                                           tr( "Select posters' folder" ),
                                                           ePostersFolder->text() );

    if( !newFolder.isEmpty() ) {
        ePostersFolder->setText( newFolder );
    }
}

/*************************************************************************************************
 *  "Application" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
    // Signals
    connect( cbLanguage, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsNeedReboot() ) );
    connect( cbStyle, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cbToolbarStyle, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( eExternalPlayer, SIGNAL( textChanged(QString) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( bSelectExternalPlayer, SIGNAL( clicked() ), this, SLOT( SelectExternalPlayer() ) );
    connect( bExternalPlayerDefault, SIGNAL( clicked() ), this, SLOT( SetDefaultExternalPlayer() ) );

    // Language ComboBox
    cbLanguage->addItem( tr( "<Auto>" ) );

    foreach( Alexandra::Locale locale, Alexandra::supportedLocales ) {
        cbLanguage->addItem( locale.title );
    }

    // Style ComboBox
    cbStyle->addItem( tr( "<Default>" ) );

    foreach( QString style, appStyles ) {
        cbStyle->addItem( style );
    }

    // Toolbar style ComboBox
    foreach( ToolStyle toolStyle, toolStyles ) {
        cbToolbarStyle->addItem( toolStyle.name );
    }

}

void SettingsWindow::ReconfigureApplicationTab()
{
    QString appStyle = settings->GetApplicationStyle();

    if( appStyle.isEmpty() ) {
        cbStyle->setCurrentIndex( 0 );
    } else {
        cbStyle->setCurrentIndex( appStyles.indexOf( appStyle ) + 1 );
    }

    eExternalPlayer->setText( settings->GetApplicationExternalPlayer() );
    cbToolbarStyle->setCurrentIndex( settings->GetMainWindowToolbarStyle() );

    isNeedReboot = false;
    isSettingsChanged = false;
}

/*************************************************************************************************
 *  "Database" tab settings                                                                       *
  *************************************************************************************************/

void SettingsWindow::ConfigureDatabaseTab()
{
    // Signals
    connect( eDatabaseFile, SIGNAL( textChanged(QString) ), this, SLOT( SetIsDatabaseSettingsChanged() ) );
    connect( bOpenDatabaseFile, SIGNAL( clicked() ), this, SLOT( OpenDatabaseFile() ) );
    connect( cCheckFilesAtStartup, SIGNAL( toggled(bool) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( bSelectColorUnavailable, SIGNAL( clicked() ), this, SLOT( SelectColorUnavailable() ) );
    connect( bCreateDatabase, SIGNAL( clicked() ), this, SLOT( CreateDatabase() ) );
    connect( bEraseDatabase, SIGNAL( clicked() ), this, SLOT( EraseDatabaseQuestion() ) );
    connect( ePostersFolder, SIGNAL( textChanged(QString) ), this, SLOT( SetIsDatabaseSettingsChanged() ) );
    connect( bOpenPostersFolder, SIGNAL( clicked() ), this, SLOT( OpenPostersFolder() ) );
    connect( cScalePoster, SIGNAL( toggled(bool) ), sbScaleToHeight, SLOT( setEnabled(bool) ) );
    connect( cScalePoster, SIGNAL( toggled(bool) ), this,  SLOT( SetIsSettingsChanged() ) );
    connect( sbScaleToHeight, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
}

void SettingsWindow::ReconfigureDatabaseTab()
{
    eDatabaseFile->setText( settings->GetApplicationDatabaseFile() );
    cCheckFilesAtStartup->setChecked( settings->GetFilmsListCheckFilesOnStartup() );
    ePostersFolder->setText( settings->GetFilmsListPostersDir() );

    int s = settings->GetFilmsListScalePosters();

    if( s == 0 ) {
        cScalePoster->setChecked( false );
        sbScaleToHeight->setEnabled( false );
    } else {
        cScalePoster->setChecked( true );
        sbScaleToHeight->setEnabled( true );
        sbScaleToHeight->setValue( s );
    }

    isDatabaseSettingsChanged = false;
}
