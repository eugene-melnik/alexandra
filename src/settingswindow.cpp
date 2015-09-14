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

SettingsWindow::SettingsWindow( AlexandraSettings* s, QWidget* parent )
    : QDialog( parent ), settings( s )
{
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
        settings->SetMainWindowToolbarStyle( cbToolbarStyle->currentIndex() );

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
        settings->SetApplicationShowSplashScreen( cShowSplashScreen->isChecked() );
        settings->SetApplicationLocaleIndex( cbLanguage->currentIndex() - 1 );

        settings->SetExternalPlayer( eExternalPlayer->text() );
        settings->SetPlayerDoubleClickBehavior( playerBehaviors.key( cbDoubleClickBehavior->currentText() ) );

        settings->SetDatabaseFilePath( eDatabaseFile->text() );
        settings->SetCheckFilesOnStartup( cCheckFilesAtStartup->isChecked() );

        settings->SetPostersDirPath( ePostersFolder->text() );
        settings->SetPosterSavingFormat( savingFormats[ cbSavingFormat->currentIndex() ].format );
        settings->SetPosterSavingQuality( savingFormats[ cbSavingFormat->currentIndex() ].quality );
        settings->SetScalePostersToHeight( cScalePoster->isChecked() ? sbScaleToHeight->value() : 0 );

        // Shortcuts tab
        for( auto& s : shortcuts )
        {
            QString currentKey = s.keyEdit->keySequence().toString();
            s.SetSetting( settings, currentKey );
        }

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

void SettingsWindow::ShortcutsKeySequenceHandler()
{
    QKeySequenceEdit* currentKeyEdit = dynamic_cast<QKeySequenceEdit*>( sender() );
    QString currentKey = currentKeyEdit->keySequence().toString();

    if( CheckForDuplicates( currentKey, currentKeyEdit->objectName() ) )
    {
        currentKeyEdit->clear();
    }

    SetIsSettingsChanged();
}

void SettingsWindow::ShortcutsDefaultButtonHandler()
{
    for( auto& s : shortcuts )
    {
        if( s.buttonDefault == sender() )
        {
            QKeySequence k( s.defaultKey );
            s.keyEdit->setKeySequence( k );

            if( CheckForDuplicates( s.defaultKey, s.keyEdit->objectName() ) )
            {
                s.keyEdit->clear();
            }

            SetIsSettingsChanged();
            return;
        }
    }
}

void SettingsWindow::ShortcutsClearButtonHandler()
{
    for( auto& s : shortcuts )
    {
        if( s.buttonClear == sender() )
        {
            s.keyEdit->clear();
            break;
        }
    }

    SetIsSettingsChanged();
}

bool SettingsWindow::CheckForDuplicates( const QString& currentKey, const QString& objName )
{
    for( auto& s : shortcuts )
    {
        if( s.keyEdit->objectName() != objName                          // Not the same object
                && s.keyEdit->keySequence().toString() == currentKey )  // Key sequences equal
        {
            QMessageBox::warning( this, tr( "Shortcuts" ), tr( "This shortcut is already in use!" ) );
            return( true );
        }
    }

    return( false );
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
    for( const QString& style : appStyles )
    {
        cbStyle->addItem( style );
    }

    // Theme ComboBox
    for( const Alexandra::Theme& theme : Alexandra::themes )
    {
        cbTheme->addItem( theme.name );
    }

    // Toolbar style ComboBox
    toolStyles.insert( Qt::ToolButtonIconOnly,       tr( "Icon only" ) );
    toolStyles.insert( Qt::ToolButtonTextOnly,       tr( "Text only" ) );
    toolStyles.insert( Qt::ToolButtonTextBesideIcon, tr( "Text beside icon" ) );
    toolStyles.insert( Qt::ToolButtonTextUnderIcon,  tr( "Text under icon" ) );
    toolStyles.insert( Qt::ToolButtonFollowStyle,    tr( "<Follow system style>" ) );

    cbToolbarStyle->addItems( toolStyles.values() );
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
    // Splash screen
    connect( cShowSplashScreen, &QCheckBox::toggled, this, &SettingsWindow::SetIsSettingsChanged );

    // Language
    connect( cbLanguage, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsNeedReboot() ) );

    // Player
    connect( eExternalPlayer, &QLineEdit::textChanged, this, &SettingsWindow::SetIsSettingsChanged );
    connect( bSelectExternalPlayer, &QPushButton::clicked, this, &SettingsWindow::SelectExternalPlayer );
    connect( bExternalPlayerDefault, &QPushButton::clicked, this, &SettingsWindow::SetDefaultExternalPlayer );
    connect( cbDoubleClickBehavior, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );

    playerBehaviors.insert( "auto",        tr( "<Auto>" ) );
    playerBehaviors.insert( "play",        tr( "Play" ) );
    playerBehaviors.insert( "add-to-list", tr( "Add to playlist" ) );

    cbDoubleClickBehavior->addItems( playerBehaviors.values() );

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
    for( const Alexandra::Locale& locale : Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title + " (" + locale.name + ")" );
    }

    // Saving format ComboBox
    for( const PosterFormat& f : savingFormats )
    {
        cbSavingFormat->addItem( f.title );
    }

#ifdef PORTABLE_VERSION
    // Database
    labelDatabaseFile->setEnabled( false );
    eDatabaseFile->setEnabled( false );
    bOpenDatabaseFile->setEnabled( false );
    // Posters
    labelPostersFolder->setEnabled( false );
    ePostersFolder->setEnabled( false );
    bOpenPostersFolder->setEnabled( false );
#endif
}

void SettingsWindow::ReconfigureApplicationTab()
{
    cShowSplashScreen->setChecked( settings->GetApplicationShowSplashScreen() );
    cbLanguage->setCurrentIndex( settings->GetApplicationLocaleIndex() + 1 );

    // Player
    eExternalPlayer->setText( settings->GetExternalPlayer() );
    cbDoubleClickBehavior->setCurrentText( playerBehaviors.value( settings->GetPlayerDoubleClickBehavior() ) );

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
    // Main setup, large and terrible
    shortcuts =
    {
        // Add, Edit, Remove
        { "Ctrl+A",     kseAddFilm,     bDefaultAddFilm,    bClearAddFilm,      &AlexandraSettings::GetShortcutAddFilm,     &AlexandraSettings::SetShortcutAddFilm },
        { "Ctrl+E",     kseEditFilm,    bDefaultEditFilm,   bClearEditFilm,     &AlexandraSettings::GetShortcutEditFilm,    &AlexandraSettings::SetShortcutEditFilm },
        { "Ctrl+Del",   kseRemoveFilm,  bDefaultRemoveFilm, bClearRemoveFilm,   &AlexandraSettings::GetShortcutRemoveFilm,  &AlexandraSettings::SetShortcutRemoveFilm },
        // Random, Quick search, Search
        { "Ctrl+R",     kseSelectRandomFilm,    bDefaultSelectRandomFilm,       bClearSelectRandomFilm,     &AlexandraSettings::GetShortcutSelectRandomFilm,    &AlexandraSettings::SetShortcutSelectRandomFilm },
        { "Backspace",  kseActivateQuickSearch, bDefaultActivateQuickSearch,    bClearActivateQuickSearch,  &AlexandraSettings::GetShortcutActivateQuickSearch, &AlexandraSettings::SetShortcutActivateQuickSearch },
        { "Ctrl+F",     kseAdvancedSearch,      bDefaultAdvancedSearch,         bClearAdvancedSearch,       &AlexandraSettings::GetShortcutAdvancedSearch,      &AlexandraSettings::SetShortcutAdvancedSearch },
        // Play
        { "Alt+Return", ksePlay, bDefaultPlay, bClearPlay, &AlexandraSettings::GetShortcutPlay, &AlexandraSettings::SetShortcutPlay },
        // Settings, Toolbar, Fullscreen
        { "Ctrl+P", kseSettings,        bDefaultSettings,       bClearSettings,         &AlexandraSettings::GetShortcutSettings,        &AlexandraSettings::SetShortcutSettings },
        { "Ctrl+T", kseShowToolbar,     bDefaultShowToolbar,    bClearShowToolbar,      &AlexandraSettings::GetShortcutShowToolbar,     &AlexandraSettings::SetShortcutShowToolbar },
        { "F11",    kseShowFullscreen,  bDefaultShowFullscreen, bClearShowFullscreen,   &AlexandraSettings::GetShortcutShowFullscreen,  &AlexandraSettings::SetShortcutShowFullscreen },
        // Exit
        { "Ctrl+Q", kseExit, bDefaultExit, bClearExit, &AlexandraSettings::GetShortcutExit, &AlexandraSettings::SetShortcutExit }
    };

    // Signals
    for( auto& s : shortcuts )
    {
        // Key sequence changed
        connect( s.keyEdit, &QKeySequenceEdit::editingFinished, this, &SettingsWindow::ShortcutsKeySequenceHandler );
        // Default button clicked
        connect( s.buttonDefault, &QPushButton::clicked, this, &SettingsWindow::ShortcutsDefaultButtonHandler );
        // Clear button clicked
        connect( s.buttonClear, &QPushButton::clicked, this, &SettingsWindow::ShortcutsClearButtonHandler );
    }
}

void SettingsWindow::ReconfigureShortcutsTab()
{
    for( auto& s : shortcuts )
    {
        QKeySequence k( s.GetSetting( settings) );
        s.keyEdit->setKeySequence( k );
    }
}
