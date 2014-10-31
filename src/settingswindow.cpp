#include "settingswindow.h"
#include "version.h"

SettingsWindow::SettingsWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );
    ConfigureLanguageCB();

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

    const QList<ToolStyle> toolStyle = { { tr("Icon only"),             Qt::ToolButtonIconOnly },
                                         { tr("Text only"),             Qt::ToolButtonTextOnly },
                                         { tr("Text beside icon"),      Qt::ToolButtonTextBesideIcon },
                                         { tr("Text under icon"),       Qt::ToolButtonTextUnderIcon },
                                         { tr("Follow system style"),   Qt::ToolButtonFollowStyle } };

    foreach ( ToolStyle ts, toolStyle )
    {
        cbToolbarStyle->addItem( ts.name );
    }
}
