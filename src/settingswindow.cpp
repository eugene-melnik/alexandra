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
    // filmslist tab
    connect( bSelectColorViewed, SIGNAL( clicked() ), this, SLOT( SelectColorViewed() ) );
    connect( bSelectColorFavourite, SIGNAL( clicked() ), this, SLOT( SelectColorFavourite() ) );
    connect( bSelectColorUnavailable, SIGNAL( clicked() ), this, SLOT( SelectColorUnavailable() ) );
}

void SettingsWindow::showEvent( QShowEvent* event )
{
    ConfigureApplicationTab();
    ConfigureDatabaseTab();
    ConfigureFilmslistTab();
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
    // filmslist tab
    settings->setValue( "FilmList/HighlightViewed", cHighlightViewed->isChecked() );
    settings->setValue( "FilmList/HighlightFavourite", cHighlightFavourite->isChecked() );
    settings->setValue( "FilmList/HighlightUnavailable", cHighlightUnavailable->isChecked() );

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

void SettingsWindow::SelectColorViewed()
{
    // TODO
    QColor oldColor = settings->value( "FilmList/ViewedColor" ).toUInt();
    QColor newColor = QColorDialog::getColor( oldColor, this );
    settings->setValue( "FilmList/ViewedColor", newColor.rgba() );
}

void SettingsWindow::SelectColorFavourite()
{
    // TODO
    QColor oldColor = settings->value( "FilmList/FavouriteColor" ).toUInt();
    QColor newColor = QColorDialog::getColor( oldColor, this );
    settings->setValue( "FilmList/FavouriteColor", newColor.rgba() );
}

void SettingsWindow::SelectColorUnavailable()
{
    // TODO
    QColor oldColor = settings->value( "FilmList/UnavailableColor" ).toUInt();
    QColor newColor = QColorDialog::getColor( oldColor, this );
    settings->setValue( "FilmList/UnavailableColor", newColor.rgba() );
}

/*************************************************************************************************
 *  "Application" tab settings                                                                    *
  *************************************************************************************************/

void SettingsWindow::ConfigureApplicationTab()
{
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
 *  "Database" tab settings                                                                       *
  *************************************************************************************************/

void SettingsWindow::ConfigureDatabaseTab()
{
    eDatabaseFile->setText( settings->value( "Application/DatabaseFile" ).toString() );
}

/*************************************************************************************************
 *  "Filmslist" tab settings                                                                      *
  *************************************************************************************************/

void SettingsWindow::ConfigureFilmslistTab()
{
    cHighlightViewed->setChecked( settings->value( "FilmList/HighlightViewed", false ).toBool() );
    cHighlightFavourite->setChecked( settings->value( "FilmList/HighlightFavourite", false ).toBool() );
    cHighlightUnavailable->setChecked( settings->value( "FilmList/HighlightUnavailable", false ).toBool() );
}
