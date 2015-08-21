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
#include <QDataStream>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QFontDialog>
#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QRadioButton>
#include <QSpinBox>

SettingsWindow::SettingsWindow( AlexandraSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    setupUi( this );
    connect( bOk, &QPushButton::clicked, this, &SettingsWindow::OkButtonClicked );

    ConfigureAppearanceTab();
    ConfigureApplicationTab();
    ConfigureShortcutsTab();
}

void SettingsWindow::show()
{
    ReconfigureAppearanceTab();
    ReconfigureApplicationTab();
    ReconfigureShortcutsTab();

    isViewChanged = false;
    isDatabaseSettingsChanged = false;
    isSettingsChanged = false;
    isNeedReboot = false;

    twMain->setCurrentIndex( 0 );
    QDialog::show();
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
        settings->SetApplicationFont( bFontSelect->font().toString() );

        if( cbStyle->currentIndex() == 0 )
        {
            settings->SetApplicationStyleName( "" );
        }
        else
        {
            settings->SetApplicationStyleName( cbStyle->currentText() );
        }

        settings->SetApplicationThemeIndex( cbTheme->currentIndex() );
        settings->SetMainWindowToolbarStyle( toolStyles[ cbToolbarStyle->currentIndex() ].style  );

        if( rbListView->isChecked() )
        {
            settings->SetFilmsViewMode( Alexandra::ListMode );
            settings->SetListFontSize( sbListFontSize->value() );
            settings->SetListRowHeight( sbListRowHeight->value() );
        }
        else
        {
            settings->SetFilmsViewMode( Alexandra::GridMode );
            settings->SetGridItemSize( sbGridImageSize->value() );
            settings->SetGridFontSize( sbGridFontSize->value() );
            settings->SetGridShowTooltip( cShowTooltip->isChecked() );
        }

        settings->SetMainWindowShowRightPanel( cShowRightPanel->isChecked() );

        // Application tab
        settings->SetApplicationLocaleIndex( cbLanguage->currentIndex() - 1 );
        settings->SetExternalPlayer( eExternalPlayer->text() );

        settings->SetDatabaseFilePath( eDatabaseFile->text() );
        settings->SetCheckFilesOnStartup( cCheckFilesAtStartup->isChecked() );

        settings->SetPostersDirPath( ePostersFolder->text() );
        settings->SetPosterSavingFormat( savingFormats[ cbSavingFormat->currentIndex() ].format );
        settings->SetPosterSavingQuality( savingFormats[ cbSavingFormat->currentIndex() ].quality );
        settings->SetScalePostersToHeight( cScalePoster->isChecked() ? sbScaleToHeight->value() : 0 );

        // Shortcuts tab
        settings->SetShortcutPlay( ksePlay->keySequence().toString() );

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

void SettingsWindow::StyleChanged()
{
    if( cbStyle->currentIndex() == 1 ) // "Theme"
    {
        labelTheme->setEnabled( true );
        cbTheme->setEnabled( true );
    }
    else
    {
        labelTheme->setEnabled( false );
        cbTheme->setEnabled( false );
    }
}

void SettingsWindow::SelectFont()
{
    bool isFontSelected;
    QFont newFont = QFontDialog::getFont( &isFontSelected, bFontSelect->font(), this );

    if( isFontSelected )
    {
        bFontSelect->setFont( newFont );
        bFontSelect->setText( newFont.family() + QString( " %1" ).arg( newFont.pointSize() ) );
        SetIsSettingsChanged();
    }
}

void SettingsWindow::SelectFontDefault()
{
    QFont defaultFont = QFontDatabase::systemFont( QFontDatabase::GeneralFont );
    bFontSelect->setFont( defaultFont );
    bFontSelect->setText( defaultFont.family() + QString( " %1" ).arg( defaultFont.pointSize() ) );
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
        QFile file( databaseFileName );

        if( file.open( QIODevice::ReadOnly ) )
        {
            // Reading from file
            QDataStream stream( &file );
            QString databaseHeader;
            quint8 databaseVersion;

            stream >> databaseHeader;
            stream >> databaseVersion;
            file.close();

            if( databaseHeader != Alexandra::databaseHeader
                    || databaseVersion != Alexandra::databaseVersion )
            {
                QMessageBox::warning( this,
                                      tr( "Database" ),
                                      tr( "Not a database file or incorrect version!" ) );
                return;
            }

        }

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

        // Remove file if exists
        QFile newDb( databaseFileName );

        if( newDb.exists() )
        {
            newDb.remove();
        }

        // Posters dir
        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";

        int res = QMessageBox::question( this,
                                         tr( "Create database" ),
                                         tr( "Would you like to set the directory for posters next to the database file?" ),
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

        if( res == QMessageBox::Yes )
        {
            ePostersFolder->setText( postersDir );
        }

        SetIsDatabaseSettingsChanged();
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
    // Global
    connect( bFontSelect, &QPushButton::clicked, this, &SettingsWindow::SelectFont );
    connect( bFontSelectDefault, &QPushButton::clicked, this, &SettingsWindow::SelectFontDefault );
    connect( bFontSelectDefault, &QPushButton::clicked, this, &SettingsWindow::SetIsSettingsChanged );
    connect( cbStyle, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cbStyle, SIGNAL( currentIndexChanged(int) ), this, SLOT( StyleChanged() ) );
    connect( cbTheme, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cbToolbarStyle, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );

    // View mode
    connect( rbListView, &QRadioButton::toggled, this, &SettingsWindow::SetIsViewChanged );
    connect( sbListFontSize, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( sbListRowHeight, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );

    connect( rbGridView, &QRadioButton::toggled, this, &SettingsWindow::SetIsViewChanged );
    connect( sbGridImageSize, SIGNAL( valueChanged(int) ), this, SLOT( SetIsViewChanged() ) );
    connect( sbGridFontSize, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cShowTooltip, &QCheckBox::toggled, this, &SettingsWindow::SetIsViewChanged );

    connect( cShowRightPanel, &QCheckBox::toggled, this,  &SettingsWindow::SetIsSettingsChanged );

    // Style ComboBox
    foreach( QString style, appStyles )
    {
        cbStyle->addItem( style );
    }

    // Theme ComboBox
    foreach( Alexandra::Theme theme, Alexandra::themes )
    {
        cbTheme->addItem( theme.name );
    }

    // Toolbar style ComboBox
    foreach( ToolStyle toolStyle, toolStyles )
    {
        cbToolbarStyle->addItem( toolStyle.name );
    }
}

void SettingsWindow::ReconfigureAppearanceTab()
{
    // Font
    QFont oldFont;
    oldFont.fromString( settings->GetApplicationFont() );
    bFontSelect->setText( oldFont.family() + QString( " %1" ).arg( oldFont.pointSize() ) );
    bFontSelect->setFont( oldFont );

    // Style ComboBox
    QString appStyle = settings->GetApplicationStyleName();

    if( appStyle.isEmpty() )
    {
        cbStyle->setCurrentIndex( 0 );
    }
    else
    {
        cbStyle->setCurrentIndex( appStyles.indexOf( appStyle ) + 1 ); // skeep "Default"
    }

    // Themes ComboBox
    cbTheme->setCurrentIndex( settings->GetApplicationThemeIndex() );

    // Toolbar style ComboBox
    cbToolbarStyle->setCurrentIndex( settings->GetMainWindowToolbarStyle() );

    // View mode
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
    cShowTooltip->setChecked( settings->GetGridShowTooltip() );

    cShowRightPanel->setChecked( settings->GetMainWindowShowRightPanel() );
}

/*************************************************************************************************
 *  "Application" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
    // Language
    connect( cbLanguage, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsNeedReboot() ) );

    // Player
    connect( eExternalPlayer, &QLineEdit::textChanged, this, &SettingsWindow::SetIsSettingsChanged );
    connect( bSelectExternalPlayer, &QPushButton::clicked, this, &SettingsWindow::SelectExternalPlayer );
    connect( bExternalPlayerDefault, &QPushButton::clicked, this, &SettingsWindow::SetDefaultExternalPlayer );

    // Database
    connect( eDatabaseFile, &QLineEdit::textChanged, this, &SettingsWindow::SetIsDatabaseSettingsChanged );
    connect( bOpenDatabaseFile, &QPushButton::clicked, this, &SettingsWindow::OpenDatabaseFile );
    connect( cCheckFilesAtStartup, &QCheckBox::toggled, this, &SettingsWindow::SetIsSettingsChanged );
    connect( bSelectColorUnavailable, &QPushButton::clicked, this, &SettingsWindow::SelectColorUnavailable );
    connect( bCreateDatabase, &QPushButton::clicked, this, &SettingsWindow::CreateDatabase );
    connect( bEraseDatabase, &QPushButton::clicked, this, &SettingsWindow::EraseDatabaseQuestion );

    // Posters
    connect( ePostersFolder, &QLineEdit::textChanged, this, &SettingsWindow::SetIsDatabaseSettingsChanged );
    connect( bOpenPostersFolder, &QPushButton::clicked, this, &SettingsWindow::OpenPostersFolder );
    connect( cScalePoster, &QCheckBox::toggled, this,  &SettingsWindow::SetIsSettingsChanged );
    connect( sbScaleToHeight, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cbSavingFormat, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );

    // Language ComboBox
    foreach( Alexandra::Locale locale, Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title + " (" + locale.name + ")" );
    }

    // Saving format ComboBox
    foreach( PosterFormat f, savingFormats )
    {
        cbSavingFormat->addItem( f.title );
    }

}

void SettingsWindow::ReconfigureApplicationTab()
{
    cbLanguage->setCurrentIndex( settings->GetApplicationLocaleIndex() + 1 );

    eExternalPlayer->setText( settings->GetExternalPlayer() );

    // Database
    eDatabaseFile->setText( settings->GetDatabaseFilePath() );
    cCheckFilesAtStartup->setChecked( settings->GetCheckFilesOnStartup() );

    // Posters
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

    // Saving format
    QString savedFormat = settings->GetPosterSavingFormat();
    int savedQuality = settings->GetPosterSavingQuality();

    for( int i = 0; i < savingFormats.size(); i++ )
    {
        if( savingFormats[i].format == savedFormat
                && savingFormats[i].quality == savedQuality )
        {
            cbSavingFormat->setCurrentIndex( i );
            break;
        }
    }
}

/*************************************************************************************************
 *  "Shortcuts" tab settings                                                                      *
  *************************************************************************************************/

void SettingsWindow::ConfigureShortcutsTab()
{
    connect( ksePlay, &QKeySequenceEdit::keySequenceChanged, this, &SettingsWindow::SetIsSettingsChanged );
    connect( bDefaultPlayShortcut, &QPushButton::clicked, this, &SettingsWindow::SetShortcutPlayDefault );
    connect( bDefaultPlayShortcut, &QPushButton::clicked, this, &SettingsWindow::SetIsSettingsChanged );
}

void SettingsWindow::ReconfigureShortcutsTab()
{
    ksePlay->setKeySequence( QKeySequence( settings->GetShortcutPlay() ) );
}
