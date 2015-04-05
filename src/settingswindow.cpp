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
#include <QRadioButton>
#include <QSpinBox>

SettingsWindow::SettingsWindow( AlexandraSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    setupUi( this );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );

    ConfigureAppearanceTab();
    ConfigureApplicationTab();
    ConfigureDatabaseTab();
}

void SettingsWindow::showEvent( QShowEvent* event )
{
    ReconfigureAppearanceTab();
    ReconfigureApplicationTab();
    ReconfigureDatabaseTab();

    isViewChanged = false;
    isDatabaseSettingsChanged = false;
    isSettingsChanged = false;
    isNeedReboot = false;

    event->accept();
}

/*************************************************************************************************
 *  Events' handlers (slots)                                                                      *
  *************************************************************************************************/

// Window

void SettingsWindow::OkButtonClicked()
{
    // Show reboot message (if necessary)
    if( isNeedReboot )
    {
        QMessageBox::information( this, tr( "Settings" ), tr( "For taking all settings, restart the application." ) );
    }

    // Hide window
    close();

    // Saving settings
    if( isSettingsChanged )
    {
        // Appearance tab
        if( rbListView->isChecked() )
        {
            settings->SetFilmsViewMode( Alexandra::ListMode );
        }
        else
        {
            settings->SetFilmsViewMode( Alexandra::GridMode );
        }

        settings->SetListFontSize( sbListFontSize->value() );
        settings->SetListRowHeight( sbListRowHeight->value() );

        settings->SetGridItemSize( sbGridImageSize->value() );
        settings->SetGridFontSize( sbGridFontSize->value() );
        settings->SetGridColumnCount( sbGridNumberOfColumns->value() );
        settings->SetGridCutTextAfter( sbGridCutTextAfter->value() );
        settings->SetGridShowTooltip( cShowTooltip->isChecked() );

        settings->SetMainWindowShowRightPanel( cShowRightPanel->isChecked() );

        // Application tab
        settings->SetApplicationLocaleIndex( cbLanguage->currentIndex() - 1 );

        if( cbStyle->currentIndex() == 0 )
        {
            settings->SetApplicationStyleName( "" );
        }
        else
        {
            settings->SetApplicationStyleName( cbStyle->currentText() );
        }

        settings->SetMainWindowToolbarStyle( toolStyles[ cbToolbarStyle->currentIndex() ].style  );
        settings->SetExternalPlayer( eExternalPlayer->text() );
        // Database tab
        settings->SetDatabaseFilePath( eDatabaseFile->text() );
        settings->SetCheckFilesOnStartup( cCheckFilesAtStartup->isChecked() );
        settings->SetPostersDirPath( ePostersFolder->text() );
        settings->SetScalePostersToHeight( cScalePoster->isChecked() ? sbScaleToHeight->value() : 0 );
        settings->sync();

        emit SettingsChanged();

        if( isDatabaseSettingsChanged )
        {
            emit DatabaseSettingsChanged();
        }
        if( isViewChanged )
        {
            emit ViewChanged();
        }
    }
}

void SettingsWindow::SetIsSettingsChanged()
{
    isSettingsChanged = true;
}

void SettingsWindow::SetIsViewChanged()
{
    SetIsSettingsChanged();
    isViewChanged = true;
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

    if( !externalPlayer.isEmpty() )
    {
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
#endif
}

    // Database tab

void SettingsWindow::OpenDatabaseFile()
{
    QString databaseFileName = QFileDialog::getOpenFileName( this,
                                                             tr( "Select database file" ),
                                                             eDatabaseFile->text(),
                                                             tr( "Alexandra DB (*.adat)" ) );

    if( !databaseFileName.isEmpty() )
    {
        eDatabaseFile->setText( databaseFileName );

        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";

        if( QDir( postersDir ).exists() )
        {
            int res = QMessageBox::question( this,
                                             tr( "Settings" ),
                                             tr( "Would you like to set the catalog of posters is \"%1\"?" ).arg( postersDir ),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            if( res == QMessageBox::Yes )
            {
                ePostersFolder->setText( postersDir );
            }
        }
    }
}

void SettingsWindow::SelectColorUnavailable()
{
    QColor oldColor = settings->GetUnavailableFileColor();
    QColor newColor = QColorDialog::getColor( oldColor, this );

    // If cancel button pressed
    if( newColor.isValid() )
    {
        settings->SetUnavailableFileColor( newColor.rgba() );
        isDatabaseSettingsChanged = true;
    }
}

void SettingsWindow::CreateDatabase()
{
    QString databaseFileName = QFileDialog::getSaveFileName( this,
                                                             tr( "Create database" ),
                                                             "database.adat",
                                                             tr( "Alexandra DB (*.adat)" ) );

    if( !databaseFileName.isEmpty() )
    {
        eDatabaseFile->setText( databaseFileName );

        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";

        if( !QDir( postersDir ).exists() )
        {
            int res = QMessageBox::question( this,
                                             tr( "Create database" ),
                                             tr( "Would you like to set the catalog of posters next to the database file?" ),
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

            if( res == QMessageBox::Yes )
            {
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

    if( res == QMessageBox::Yes )
    {
        emit EraseDatabase();
    }
}

void SettingsWindow::OpenPostersFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory( this,
                                                           tr( "Select posters' folder" ),
                                                           ePostersFolder->text() );

    if( !newFolder.isEmpty() )
    {
        ePostersFolder->setText( newFolder );
    }
}

/*************************************************************************************************
 *  "Appearance" tab settings                                                                     *
  *************************************************************************************************/

void SettingsWindow::ConfigureAppearanceTab()
{
    connect( rbListView, SIGNAL( toggled(bool) ), this, SLOT( SetIsViewChanged() ) );
    connect( sbListFontSize, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( sbListRowHeight, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );

    connect( rbGridView, SIGNAL( toggled(bool) ), this, SLOT( SetIsViewChanged() ) );
    connect( sbGridImageSize, SIGNAL( valueChanged(int) ), this, SLOT( SetIsViewChanged() ) );
    connect( sbGridFontSize, SIGNAL( valueChanged(int) ), this, SLOT( SetIsViewChanged() ) );
    connect( sbGridNumberOfColumns, SIGNAL( valueChanged(int) ), this, SLOT( SetIsViewChanged() ) );
    connect( sbGridCutTextAfter, SIGNAL( valueChanged(int) ), this, SLOT( SetIsViewChanged() ) );
    connect( cShowTooltip, SIGNAL( toggled(bool) ), this,  SLOT( SetIsViewChanged() ) );

    connect( cShowRightPanel, SIGNAL( toggled(bool) ), this,  SLOT( SetIsSettingsChanged() ) );
}

void SettingsWindow::ReconfigureAppearanceTab()
{
    if( settings->GetFilmsViewMode() == Alexandra::ListMode )
    {
        rbListView->setChecked( true );
    }
    else // Alexandra::GridMode
    {
        rbGridView->setChecked( true );
    }

    // List view
    sbListFontSize->setValue( settings->GetListFontSize() );
    sbListRowHeight->setValue( settings->GetListRowHeight() );

    // Grid view
    sbGridImageSize->setValue( settings->GetGridItemSize() );
    sbGridFontSize->setValue( settings->GetGridFontSize() );
    sbGridNumberOfColumns->setValue( settings->GetGridColumnCount() );
    sbGridCutTextAfter->setValue( settings->GetGridCutTextAfter() );
    cShowTooltip->setChecked( settings->GetGridShowTooltip() );

    cShowRightPanel->setChecked( settings->GetMainWindowShowRightPanel() );
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
    foreach( Alexandra::Locale locale, Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title + " (" + locale.name + ")" );
    }

    // Style ComboBox
    foreach( QString style, appStyles )
    {
        cbStyle->addItem( style );
    }

    // Toolbar style ComboBox
    foreach( ToolStyle toolStyle, toolStyles )
    {
        cbToolbarStyle->addItem( toolStyle.name );
    }

}

void SettingsWindow::ReconfigureApplicationTab()
{
    cbLanguage->setCurrentIndex( settings->GetApplicationLocaleIndex() + 1 );

    QString appStyle = settings->GetApplicationStyleName();

    if( appStyle.isEmpty() )
    {
        cbStyle->setCurrentIndex( 0 );
    }
    else
    {
        cbStyle->setCurrentIndex( appStyles.indexOf( appStyle ) + 1 );
    }

    eExternalPlayer->setText( settings->GetExternalPlayer() );
    cbToolbarStyle->setCurrentIndex( settings->GetMainWindowToolbarStyle() );
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
    connect( cScalePoster, SIGNAL( toggled(bool) ), this,  SLOT( SetIsSettingsChanged() ) );
    connect( sbScaleToHeight, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
}

void SettingsWindow::ReconfigureDatabaseTab()
{
    eDatabaseFile->setText( settings->GetDatabaseFilePath() );
    cCheckFilesAtStartup->setChecked( settings->GetCheckFilesOnStartup() );
    ePostersFolder->setText( settings->GetPostersDirPath() );

    int height = settings->GetScalePosterToHeight();

    if( height == 0 )
    {
        cScalePoster->setChecked( false );
        sbScaleToHeight->setEnabled( false );
    }
    else
    {
        cScalePoster->setChecked( true );
        sbScaleToHeight->setEnabled( true );
        sbScaleToHeight->setValue( height );
    }
}
