#include "settingswindow.h"
#include "version.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QColorDialog>

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
    typedef struct {
        QString name;
        Qt::ToolButtonStyle style;
    } ToolStyle;

    const QList<ToolStyle> toolStyles = { { tr("<Follow system style>"),Qt::ToolButtonFollowStyle },
                                          { tr("Icon only"),            Qt::ToolButtonIconOnly },
                                          { tr("Text only"),            Qt::ToolButtonTextOnly },
                                          { tr("Text beside icon"),     Qt::ToolButtonTextBesideIcon },
                                          { tr("Text under icon"),      Qt::ToolButtonTextUnderIcon } };

    cbToolbarStyle->clear();

    foreach( ToolStyle toolStyle, toolStyles ) {
        cbToolbarStyle->addItem( toolStyle.name );
    }
}

/*************************************************************************************************
 *  "Database" tab settings                                                                       *
  *************************************************************************************************/

void SettingsWindow::ConfigureDatabaseTab()
{
    eDatabaseFile->setText( settings->value( "Application/DatabaseFile" ).toString() );
    cCheckFilesAtStartup->setChecked( settings->value( "FilmList/CheckFilesOnStartup", false ).toBool() );
}
