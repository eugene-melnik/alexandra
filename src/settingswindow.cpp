/*************************************************************************************************
 *                                                                                                *
 *  file: settingswindow.cpp                                                                      *
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

#include "settingswindow.h"
#include "filmslist/filmslistloader.h"
#include "parsers/parsermanager.h"
#include "version.h"

#include <QColorDialog>
#include <QDataStream>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QFontDialog>
#include <QMessageBox>
#include <QStyleFactory>


SettingsWindow::SettingsWindow( QWidget* parent ) : QDialog( parent ), settings( AlexandraSettings::GetInstance() )
{
    DebugPrintFunc( "SettingsWindow::SettingsWindow" );

    setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );

    ConfigureAppearanceTab();
    ConfigureApplicationTab();
    ConfigureShortcutsTab();
    ConfigureSourcesTab();

    connect( bApply, &QPushButton::clicked, this, &SettingsWindow::SaveSettings );
    connect( bOk, &QPushButton::clicked, this, [this] { SaveSettings(); close(); } );

    DebugPrintFuncDone( "SettingsWindow::SettingsWindow" );
}


/*************************************************************************************************
 *  "Appearance" tab                                                                              *
  *************************************************************************************************/

void SettingsWindow::ConfigureAppearanceTab()
{
      /// Font

    QFont oldFont;
    oldFont.fromString( settings->GetApplicationFont() );
    bFontSelect->setText( oldFont.family() + QString( " %1" ).arg( oldFont.pointSize() ) );
    bFontSelect->setFont( oldFont );

    connect( bFontSelect,        &QPushButton::clicked, this, &SettingsWindow::SelectFont );
    connect( bFontSelectDefault, &QPushButton::clicked, this, &SettingsWindow::SelectFontDefault );

      /// Application styles

    appStyles.append( tr("<Theme>") );
    appStyles.append( QStyleFactory::keys() );

    cbStyle->addItems( appStyles );
    QString appStyle = settings->GetApplicationStyleName();

    if( appStyle.isEmpty() )
    {
        cbStyle->setCurrentIndex( 0 );
    }
    else
    {
        cbStyle->setCurrentIndex( appStyles.indexOf(appStyle) + 1 ); // skip "Default"

        if( cbStyle->currentIndex() == 1 ) // "Theme"
        {
            labelTheme->setEnabled( true );
            cbTheme->setEnabled( true );
        }
    }

    connect( cbStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(StyleChanged()) );

      /// Themes

    for( const Alexandra::Theme& theme : Alexandra::themes )
    {
        cbTheme->addItem( theme.name );
    }

    cbTheme->setCurrentIndex( settings->GetApplicationThemeIndex() );
    connect( cbTheme, SIGNAL(currentIndexChanged(int)), this, SLOT(SetIsSettingsChanged()) );

      /// Toolbar style

    toolStyles.insert( Qt::ToolButtonIconOnly,       tr("Icon only") );
    toolStyles.insert( Qt::ToolButtonTextOnly,       tr("Text only") );
    toolStyles.insert( Qt::ToolButtonTextBesideIcon, tr("Text beside icon") );
    toolStyles.insert( Qt::ToolButtonTextUnderIcon,  tr("Text under icon") );
    toolStyles.insert( Qt::ToolButtonFollowStyle,    tr("<Follow system style>") );

    cbToolbarStyle->addItems( toolStyles.values() );
    cbToolbarStyle->setCurrentIndex( settings->GetMainWindowToolbarStyle() );
    connect( cbToolbarStyle, SIGNAL(currentIndexChanged(int)), this, SLOT( SetIsSettingsChanged()) );

      /// View mode

    if( settings->GetFilmsViewMode() == Alexandra::DetailedListMode )
    {
        rbDetailedListView->setChecked( true );
    }
    else if( settings->GetFilmsViewMode() == Alexandra::GridMode )
    {
        rbGridView->setChecked( true );
    }
    else // Alexandra::ListMode by default
    {
        rbListView->setChecked( true );
    }

      // List view
    sbListFontSize->setValue( settings->GetListFontSize() );
    sbListRowHeight->setValue( settings->GetListRowHeight() );

    connect( rbListView,      &QRadioButton::toggled,    this, &SettingsWindow::SetIsViewChanged );
    connect( sbListFontSize,  SIGNAL(valueChanged(int)), this, SLOT(SetIsSettingsChanged()) );
    connect( sbListRowHeight, SIGNAL(valueChanged(int)), this, SLOT(SetIsSettingsChanged()) );

      // Grid view
    sbGridImageSize->setValue( settings->GetGridItemSize() );
    sbGridFontSize->setValue( settings->GetGridFontSize() );
    cShowTooltip->setChecked( settings->GetGridShowTooltip() );

    connect( rbGridView,      &QRadioButton::toggled,    this, &SettingsWindow::SetIsViewChanged );
    connect( sbGridImageSize, SIGNAL(valueChanged(int)), this, SLOT(SetIsViewChanged()) );
    connect( sbGridFontSize,  SIGNAL(valueChanged(int)), this, SLOT(SetIsSettingsChanged()) );
    connect( cShowTooltip,    &QCheckBox::toggled,       this, &SettingsWindow::SetIsViewChanged );

      /// Right panel

    cShowRightPanel->setChecked( settings->GetMainWindowShowRightPanel() );
    sbPanelWidth->setValue( settings->GetMainWindowRightPanelWidth() );
    cbShowTechInfo->setChecked( settings->GetShowTechInfo() );

    connect( cShowRightPanel, &QCheckBox::toggled,       this, &SettingsWindow::SetIsSettingsChanged );
    connect( sbPanelWidth,    SIGNAL(valueChanged(int)), this, SLOT(SetIsSettingsChanged()) );
    connect( cbShowTechInfo,  &QCheckBox::toggled,       this, &SettingsWindow::SetIsSettingsChanged );
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

    SetIsSettingsChanged();
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

    SetIsSettingsChanged();
    DebugPrint( "Default font restored: " + defaultFont.toString() );
}


/*************************************************************************************************
 *  "Application" tab                                                                             *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
      /// Splash screen

    cShowSplashScreen->setChecked( settings->GetApplicationShowSplashScreen() );
    connect( cShowSplashScreen, &QCheckBox::toggled, this, &SettingsWindow::SetIsSettingsChanged );

      /// Language

    for( const Alexandra::Locale& locale : Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title + " (" + locale.name + ")" );
    }

    cbLanguage->setCurrentIndex( settings->GetApplicationLocaleIndex() + 1 );
    connect( cbLanguage, SIGNAL(currentIndexChanged(int)), this, SLOT(SetIsNeedReboot()) );

      /// Player

    playerBehaviors.insert( "0-auto",        tr("<Auto>") );
    playerBehaviors.insert( "1-add-to-list", tr("Add to playlist") );
    playerBehaviors.insert( "2-play",        tr("Play") );

    cbDoubleClickBehavior->addItems( playerBehaviors.values() );
    cbDoubleClickBehavior->setCurrentText( playerBehaviors.value( settings->GetPlayerDoubleClickBehavior() ) );
    eExternalPlayer->setText( settings->GetExternalPlayer() );

    connect( eExternalPlayer,        &QLineEdit::textChanged, this, &SettingsWindow::SetIsSettingsChanged );
    connect( bSelectExternalPlayer,  &QPushButton::clicked,   this, &SettingsWindow::SelectExternalPlayer );
    connect( bExternalPlayerDefault, &QPushButton::clicked,   this, &SettingsWindow::SetDefaultExternalPlayer );
    connect( cbDoubleClickBehavior, SIGNAL(currentIndexChanged(int)), this, SLOT(SetIsSettingsChanged()) );

      /// Database

    eDatabaseFile->setText( settings->GetDatabaseFilePath() );
    cCheckFilesAtStartup->setChecked( settings->GetCheckFilesOnStartup() );

    connect( eDatabaseFile,           &QLineEdit::textChanged, this, &SettingsWindow::SetIsDbSettingsChanged );
    connect( bOpenDatabaseFile,       &QPushButton::clicked,   this, &SettingsWindow::OpenDatabaseFile );
    connect( cCheckFilesAtStartup,    &QCheckBox::toggled,     this, &SettingsWindow::SetIsDbSettingsChanged );
    connect( bSelectColorUnavailable, &QPushButton::clicked,   this, &SettingsWindow::SelectColorUnavailable );
    connect( bCreateDatabase,         &QPushButton::clicked,   this, &SettingsWindow::CreateDatabase );
    connect( bEraseDatabase,          &QPushButton::clicked,   this, &SettingsWindow::EraseDatabaseQuestion );

      /// Posters

    ePostersFolder->setText( settings->GetPostersDirPath() );

    for( const PosterFormat& f : savingFormats )
    {
        cbSavingFormat->addItem( f.title );
    }

    QString savedFormat = settings->GetPosterSavingFormat();
    int savedQuality = settings->GetPosterSavingQuality();

    for( int i = 0; i < savingFormats.size(); i++ )
    {
        if( savingFormats[i].format == savedFormat && savingFormats[i].quality == savedQuality )
        {
            cbSavingFormat->setCurrentIndex(i);
            break;
        }
    }

    if( settings->GetScalePosterToHeight() == 0 )
    {
        cScalePoster->setChecked( false );
        sbScaleToHeight->setEnabled( false );
    }
    else
    {
        cScalePoster->setChecked( true );
        sbScaleToHeight->setEnabled( true );
        sbScaleToHeight->setValue( settings->GetScalePosterToHeight() );
    }

    connect( ePostersFolder, &QLineEdit::textChanged, this, &SettingsWindow::SetIsDbSettingsChanged );
    connect( bOpenPostersFolder, &QPushButton::clicked, this, &SettingsWindow::OpenPostersFolder );
    connect( cScalePoster, &QCheckBox::toggled, this,  &SettingsWindow::SetIsSettingsChanged );
    connect( sbScaleToHeight, SIGNAL( valueChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cbSavingFormat, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );

    #ifdef PORTABLE_VERSION
          // Database
        labelDatabaseFile->setEnabled( false );
        eDatabaseFile->setEnabled( false );
        bOpenDatabaseFile->setEnabled( false );
        bCreateDatabase->setEnabled( false );
          // Posters
        labelPostersFolder->setEnabled( false );
        ePostersFolder->setEnabled( false );
        bOpenPostersFolder->setEnabled( false );
    #endif
}


void SettingsWindow::SelectExternalPlayer()
{
    #ifdef Q_OS_WIN32
        QString filter = tr("Executable files (*.exe)");
    #else
        QString filter = tr("Executable files (*)");
    #endif

    QString externalPlayer = QFileDialog::getOpenFileName( this, tr("Select external player"), eExternalPlayer->text(), filter );

    if( !externalPlayer.isEmpty() )
    {
        eExternalPlayer->setText( externalPlayer );
    }
}


void SettingsWindow::SetDefaultExternalPlayer()
{
    #ifdef Q_OS_LINUX
        eExternalPlayer->setText( "xdg-open" );
    #elif defined(Q_OS_WIN32)
        eExternalPlayer->setText( "C:\\Program Files\\Windows Media Player\\wmplayer.exe" );
    #endif
}


void SettingsWindow::OpenDatabaseFile()
{
    QString databaseFileName = QFileDialog::getOpenFileName( this, tr("Select database file"),
                                                             eDatabaseFile->text(),
                                                             tr("Alexandra DB (*.adat)") );

    if( !databaseFileName.isEmpty() )
    {
        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";
        eDatabaseFile->setText( databaseFileName );

        if( QFile::exists( postersDir ) )
        {
            int answer = QMessageBox::question( this, tr("Settings"),
                                                tr("Would you like to set the catalog of posters is \"%1\"?").arg( postersDir ) );

            if( answer == QMessageBox::Yes )
            {
                ePostersFolder->setText( postersDir );
            }
        }
    }
}


void SettingsWindow::CreateDatabase()
{
    QString databaseFileName = QFileDialog::getSaveFileName( this, tr("Create database"), "database.adat", tr("Alexandra DB (*.adat)") );

    if( !databaseFileName.isEmpty() )
    {
        eDatabaseFile->setText( databaseFileName );
        FilmsListLoader::CreateEmptyDatabase( databaseFileName );

          // Posters dir
        QString postersDir = QFileInfo( databaseFileName ).absolutePath() + "/posters";

        int answer = QMessageBox::question( this, tr("Create database"),
                                            tr("Would you like to set the directory for posters next to the database file?"),
                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );

        if( answer == QMessageBox::Yes )
        {
            ePostersFolder->setText( postersDir );
        }

        SetIsDbSettingsChanged();
    }
}


void SettingsWindow::EraseDatabaseQuestion()
{
    int answer = QMessageBox::warning( this, tr("Erase database"),
                                       tr("Are you sure you want to erase the database and posters?"),
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::No );

    if( answer == QMessageBox::Yes )
    {
        emit EraseDatabase();
    }
}


void SettingsWindow::SelectColorUnavailable()
{
    QColor newColor = QColorDialog::getColor( settings->GetUnavailableFileColor(), this );

      // If cancel button was pressed
    if( newColor.isValid() )
    {
        settings->SetUnavailableFileColor( newColor.rgba() );
        SetIsDbSettingsChanged();
    }
}


void SettingsWindow::OpenPostersFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory( this, tr("Select posters' folder"), ePostersFolder->text() );

    if( !newFolder.isEmpty() )
    {
        ePostersFolder->setText( newFolder );
    }
}


/*************************************************************************************************
 *  "Shortcuts" tab                                                                               *
  *************************************************************************************************/

void SettingsWindow::ConfigureShortcutsTab()
{
    shortcuts =
    {
          /// Add, Edit, Remove
        { "Ctrl+A",     kseAddFilm,     bDefaultAddFilm,    bClearAddFilm,      &AlexandraSettings::GetShortcutAddFilm,     &AlexandraSettings::SetShortcutAddFilm },
        { "Ctrl+E",     kseEditFilm,    bDefaultEditFilm,   bClearEditFilm,     &AlexandraSettings::GetShortcutEditFilm,    &AlexandraSettings::SetShortcutEditFilm },
        { "Ctrl+Del",   kseRemoveFilm,  bDefaultRemoveFilm, bClearRemoveFilm,   &AlexandraSettings::GetShortcutRemoveFilm,  &AlexandraSettings::SetShortcutRemoveFilm },
          /// Random, Quick search, Search
        { "Ctrl+R",     kseSelectRandomFilm,    bDefaultSelectRandomFilm,       bClearSelectRandomFilm,     &AlexandraSettings::GetShortcutSelectRandomFilm,    &AlexandraSettings::SetShortcutSelectRandomFilm },
        { "Backspace",  kseActivateQuickSearch, bDefaultActivateQuickSearch,    bClearActivateQuickSearch,  &AlexandraSettings::GetShortcutActivateQuickSearch, &AlexandraSettings::SetShortcutActivateQuickSearch },
        { "Ctrl+F",     kseAdvancedSearch,      bDefaultAdvancedSearch,         bClearAdvancedSearch,       &AlexandraSettings::GetShortcutAdvancedSearch,      &AlexandraSettings::SetShortcutAdvancedSearch },
          /// Play
        { "Alt+Return", ksePlay, bDefaultPlay, bClearPlay, &AlexandraSettings::GetShortcutPlay, &AlexandraSettings::SetShortcutPlay },
          /// Settings, Toolbar, Fullscreen
        { "Ctrl+P", kseSettings,        bDefaultSettings,       bClearSettings,         &AlexandraSettings::GetShortcutSettings,        &AlexandraSettings::SetShortcutSettings },
        { "Ctrl+T", kseShowToolbar,     bDefaultShowToolbar,    bClearShowToolbar,      &AlexandraSettings::GetShortcutShowToolbar,     &AlexandraSettings::SetShortcutShowToolbar },
        { "F11",    kseShowFullscreen,  bDefaultShowFullscreen, bClearShowFullscreen,   &AlexandraSettings::GetShortcutShowFullscreen,  &AlexandraSettings::SetShortcutShowFullscreen },
          /// Exit
        { "Ctrl+Q", kseExit, bDefaultExit, bClearExit, &AlexandraSettings::GetShortcutExit, &AlexandraSettings::SetShortcutExit }
    };

    for( Shortcut& s : shortcuts )
    {
        s.keyEdit->setKeySequence( QKeySequence( s.GetSetting(settings) ) );

        connect( s.keyEdit,       &QKeySequenceEdit::editingFinished, this, &SettingsWindow::ShortcutsKeySequenceHandler );
        connect( s.buttonDefault, &QPushButton::clicked,              this, &SettingsWindow::ShortcutsDefaultButtonHandler );
        connect( s.buttonClear,   &QPushButton::clicked,              this, &SettingsWindow::ShortcutsClearButtonHandler );
    }
}


void SettingsWindow::ShortcutsKeySequenceHandler()
{
    QKeySequenceEdit* currentKeyEdit = static_cast<QKeySequenceEdit*>( sender() );
    QString currentKey = currentKeyEdit->keySequence().toString();

    if( HasDuplicate( currentKey, currentKeyEdit->objectName() ) )
    {
        currentKeyEdit->clear();
    }

    SetIsSettingsChanged();
    DebugPrint( currentKeyEdit->objectName() + ": " + currentKey );
}


void SettingsWindow::ShortcutsDefaultButtonHandler()
{
    for( Shortcut& s : shortcuts )
    {
        if( s.buttonDefault == sender() )
        {
            s.keyEdit->setKeySequence( QKeySequence( s.defaultKey ) );

            if( HasDuplicate( s.defaultKey, s.keyEdit->objectName() ) )
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
    for( Shortcut& s : shortcuts )
    {
        if( s.buttonClear == sender() )
        {
            DebugPrint( "Button: " + s.buttonClear->objectName() );
            s.keyEdit->clear();
            break;
        }
    }

    SetIsSettingsChanged();
}


bool SettingsWindow::HasDuplicate( QString currentKey, QString objName )
{
    for( Shortcut& s : shortcuts )
    {
        if( s.keyEdit->objectName() != objName                      // Not the same object
            && s.keyEdit->keySequence().toString() == currentKey )  // Key sequences equal
        {
            QMessageBox::warning( this, tr("Shortcuts"), tr("This shortcut is already in use!") );
            return( true );
        }
    }

    return( false );
}


/*************************************************************************************************
 *  "Sources" tab                                                                                 *
  *************************************************************************************************/

void SettingsWindow::ConfigureSourcesTab()
{
    cbDefaultOnlineSource->addItems( ParserManager().GetAvailableParsers() );
    cbDefaultOnlineSource->setCurrentIndex( settings->GetDefaultParserIndex() );
    cDownloadBigPoster->setChecked( settings->GetParsersLoadBigPoster() );
    cDownloadMoreInformation->setChecked( settings->GetParsersLoadAdvancedInfo() );

    connect( cbDefaultOnlineSource, SIGNAL( currentIndexChanged(int) ), this, SLOT( SetIsSettingsChanged() ) );
    connect( cDownloadBigPoster, &QCheckBox::toggled, this, &SettingsWindow::SetIsSettingsChanged );
    connect( cDownloadMoreInformation, &QCheckBox::toggled, this, &SettingsWindow::SetIsSettingsChanged );
}


void SettingsWindow::SaveSettings()
{
    DebugPrintFunc( "SettingsWindow::SaveSettings" );

    if( isNeedReboot )
    {
        QMessageBox::information( this, tr("Settings"), tr("For taking all settings, restart the application.") );
    }

    if( isSettingsChanged )
    {
          /// Appearance tab

        settings->SetApplicationFont( bFontSelect->font().toString() );
        settings->SetApplicationStyleName( (cbStyle->currentIndex() == 0) ? "" : cbStyle->currentText() );
        settings->SetApplicationThemeIndex( cbTheme->currentIndex() );
        settings->SetMainWindowToolbarStyle( cbToolbarStyle->currentIndex() );

        if( rbDetailedListView->isChecked() )
        {
            settings->SetFilmsViewMode( Alexandra::DetailedListMode );
            /// TODO: add more settings
        }
        else if( rbGridView->isChecked() )
        {
            settings->SetFilmsViewMode( Alexandra::GridMode );
            settings->SetGridItemSize( sbGridImageSize->value() );
            settings->SetGridFontSize( sbGridFontSize->value() );
            settings->SetGridShowTooltip( cShowTooltip->isChecked() );
        }
        else // ListMode
        {
            settings->SetFilmsViewMode( Alexandra::ListMode );
            settings->SetListFontSize( sbListFontSize->value() );
            settings->SetListRowHeight( sbListRowHeight->value() );
        }

        settings->SetMainWindowShowRightPanel( cShowRightPanel->isChecked() );
        settings->SetMainWindowRightPanelWidth( sbPanelWidth->value() );
        settings->SetShowTechInfo( cbShowTechInfo->isChecked() );

          /// Application tab

        settings->SetApplicationShowSplashScreen( cShowSplashScreen->isChecked() );
        settings->SetApplicationLocaleIndex( cbLanguage->currentIndex()-1 );

        settings->SetExternalPlayer( eExternalPlayer->text() );
        settings->SetPlayerDoubleClickBehavior( playerBehaviors.key( cbDoubleClickBehavior->currentText() ) );

        settings->SetDatabaseFilePath( eDatabaseFile->text() );
        settings->SetCheckFilesOnStartup( cCheckFilesAtStartup->isChecked() );

        settings->SetPostersDirPath( ePostersFolder->text() );
        settings->SetPosterSavingFormat( savingFormats[ cbSavingFormat->currentIndex() ].format );
        settings->SetPosterSavingQuality( savingFormats[ cbSavingFormat->currentIndex() ].quality );
        settings->SetScalePostersToHeight( cScalePoster->isChecked() ? sbScaleToHeight->value() : 0 );

          /// Shortcuts tab

        for( Shortcut& s : shortcuts )
        {
            QString currentKey = s.keyEdit->keySequence().toString();
            s.SetSetting( settings, currentKey );
        }

          /// Sources tab

        settings->SetDefaultParserIndex( cbDefaultOnlineSource->currentIndex() );
        settings->SetParsersLoadBigPoster( cDownloadBigPoster->isChecked() );
        settings->SetParsersLoadAdvancedInfo( cDownloadMoreInformation->isChecked() );

          /// Save

        settings->sync();
        emit SettingsChanged();

        if( isDbSettingsChanged )
        {
            emit DbSettingsChanged();
        }
        if( isViewChanged )
        {
            emit ViewChanged();
        }
    }

    isSettingsChanged = false;
    isDbSettingsChanged = false;
    isViewChanged = false;
    isNeedReboot = false;

    DebugPrintFuncDone( "SettingsWindow::SaveSettings" );
}

