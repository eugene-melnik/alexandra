#include "settingswindow.h"
#include "version.h"

#include <QFileDialog>
#include <QFileInfo>

SettingsWindow::SettingsWindow( QSettings* s, QWidget* parent ) : QDialog( parent )
{
    settings = s;

    setupUi( this );
    connect( buttonBox, SIGNAL( accepted() ), this, SLOT( OkButtonClicked() ) );
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
    settings->setValue( "Application/DatabaseFile", eDatabaseFile->text() );
    settings->setValue( "Application/ExternalPlayer", eExternalPlayer->text() );

    settings->sync();
    close();
    emit SettingsChanged();
}

void SettingsWindow::SetDefaultExternalPlayer()
{
#ifdef Q_OS_LINUX
    eExternalPlayer->setText( "xdg-open" );
#else
    eExternalPlayer->clear();
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

/*************************************************************************************************
 *  "Application" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
    connect( bExternalPlayerDefault, SIGNAL( clicked() ), this, SLOT( SetDefaultExternalPlayer() ) );

    eExternalPlayer->setText( settings->value( "Application/ExternalPlayer" ).toString() );

    ConfigureLanguageCB();
    ConfigureToolbarStyleCB();
}

void SettingsWindow::ConfigureLanguageCB()
{
    cbLanguage->clear();
    cbLanguage->addItem( tr( "<Auto>" ) );

    foreach( Alexandra::Locale locale, Alexandra::supportedLocales )
    {
        cbLanguage->addItem( locale.title );
    }
}

void SettingsWindow::ConfigureStyleCB()
{
    cbStyle->clear();
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

    cbToolbarStyle->clear();

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
