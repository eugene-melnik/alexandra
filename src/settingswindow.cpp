#include "settingswindow.h"
#include "version.h"

#include <QFileDialog>
#include <QFileInfo>

SettingsWindow::SettingsWindow( QSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    setupUi( this );
    ConfigureApplicationTab();
    ConfigureDatabaseTab();

    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );
}

/*************************************************************************************************
 *  Events' handlers (slots)                                                                      *
  *************************************************************************************************/

void SettingsWindow::OkButtonClicked()
{
    settings->setValue( "Application/DatabaseFile", eDatabaseFile->text() );
    settings->setValue( "Application/ExternalPlayer", eExternalPlayer->text() );

    settings->sync();
    close();
    emit SettingsChanged();
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

/*************************************************************************************************
 *  "Application" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
    ConfigureLanguageCB();
    ConfigureToolbarStyleCB();
}

void SettingsWindow::ConfigureLanguageCB()
{
    cbLanguage->addItem( tr( "<Auto>" ) );

    foreach( Alexandra::Locale locale, Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title );
    }
}

void SettingsWindow::ConfigureStyleCB()
{
    //
}

void SettingsWindow::ConfigureToolbarStyleCB()
{
    typedef struct {
        QString name;
        Qt::ToolButtonStyle style;
    } ToolStyle;

    const QList<ToolStyle> toolStyles = { { tr("<Follow system style>"),Qt::ToolButtonFollowStyle },
                                          { tr("Icon only"),            Qt::ToolButtonIconOnly },
                                          { tr("Text only"),            Qt::ToolButtonTextOnly },
                                          { tr("Text beside icon"),     Qt::ToolButtonTextBesideIcon },
                                          { tr("Text under icon"),      Qt::ToolButtonTextUnderIcon } };

    foreach( ToolStyle toolStyle, toolStyles )
    {
        cbToolbarStyle->addItem( toolStyle.name );
    }
}

/*************************************************************************************************
 *  "Database" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureDatabaseTab()
{
    connect( bOpenDatabaseFile, SIGNAL( clicked() ), this, SLOT( OpenDatabaseFile() ) );

    eDatabaseFile->setText( settings->value( "Application/DatabaseFile" ).toString() );
}
