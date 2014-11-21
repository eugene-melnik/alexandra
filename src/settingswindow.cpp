/*************************************************************************************************
 *                                                                                                *
 *  file: settingswindow.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014 Eugene Melnik <jeka7js@gmail.com>                                          *
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

SettingsWindow::SettingsWindow( QSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    // window
    setupUi( this );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );
    // application tab
    connect( bExternalPlayerDefault, SIGNAL( clicked() ), this, SLOT( SetDefaultExternalPlayer() ) );
    // database tab
    connect( bOpenDatabaseFile, SIGNAL( clicked() ), this, SLOT( OpenDatabaseFile() ) );
    connect( bSelectColorUnavailable, SIGNAL( clicked() ), this, SLOT( SelectColorUnavailable() ) );
}

void SettingsWindow::showEvent( QShowEvent* event )
{
    ConfigureApplicationTab();
    ConfigureDatabaseTab();
    event->accept();
}

/*************************************************************************************************
 *  Events' handlers (slots)                                                                      *
  *************************************************************************************************/

void SettingsWindow::OkButtonClicked()
{
    // application tab
    settings->setValue( "MainWindow/ToolbarStyle", toolStyles[ cbToolbarStyle->currentIndex() ].style  );
    settings->setValue( "Application/ExternalPlayer", eExternalPlayer->text() );
    // database tab
    settings->setValue( "Application/DatabaseFile", eDatabaseFile->text() );
    settings->setValue( "FilmList/CheckFilesOnStartup", cCheckFilesAtStartup->isChecked() );

    // save settings
    close();
    settings->sync();
    emit SettingsChanged();
}

void SettingsWindow::SetDefaultExternalPlayer()
{
#ifdef Q_OS_LINUX
    eExternalPlayer->setText( "xdg-open" );
#else
    eExternalPlayer->clear(); // dummy
#endif
}

void SettingsWindow::OpenDatabaseFile()
{
    QFileInfo currentPath( eDatabaseFile->text() );

    QString databaseFileName = QFileDialog::getOpenFileName( this,
                               tr( "Select database file" ),
                               currentPath.absoluteFilePath(),
                               tr( "Alexandra DB (*.adat)" ) );

    if( !databaseFileName.isEmpty() ) {
        eDatabaseFile->setText( databaseFileName );
    }
}

void SettingsWindow::SelectColorUnavailable()
{
    // FIX when close button press
    QColor oldColor = settings->value( "FilmList/UnavailableFileColor", QColor( "red" ).toRgb() ).toUInt();
    QColor newColor = QColorDialog::getColor( oldColor, this );
    settings->setValue( "FilmList/UnavailableFileColor", newColor.rgba() );
}

/*************************************************************************************************
 *  "Application" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
    eExternalPlayer->setText( settings->value( "Application/ExternalPlayer" ).toString() );

    // Language ComboBox
    cbLanguage->clear();
    cbLanguage->addItem( tr( "<Auto>" ) );

    foreach( Alexandra::Locale locale, Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title );
    }

    // Style ComboBox
    cbStyle->clear(); // dummy

    // Toolbar style ComboBox
    cbToolbarStyle->clear();

    foreach( ToolStyle toolStyle, toolStyles ) {
        cbToolbarStyle->addItem( toolStyle.name );
    }

    cbToolbarStyle->setCurrentIndex( settings->value( "MainWindow/ToolbarStyle", (int)Qt::ToolButtonFollowStyle ).toInt() );
}

/*************************************************************************************************
 *  "Database" tab settings                                                                       *
  *************************************************************************************************/

void SettingsWindow::ConfigureDatabaseTab()
{
    eDatabaseFile->setText( settings->value( "Application/DatabaseFile" ).toString() );
    cCheckFilesAtStartup->setChecked( settings->value( "FilmList/CheckFilesOnStartup", false ).toBool() );
}
