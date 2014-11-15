#include "aboutwindow.h"
#include "version.h"

#include <QMessageBox>
#include <QPixmap>
#include <QString>

AboutWindow::AboutWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    lAppName->setText( Alexandra::appNameGui );
    lAppVersion->setText( tr( "version %1 (build date: %2)" ).arg( Alexandra::appVersionFull, Alexandra::appBuildDate ) );

    lQtVersion->setText( tr( "<b>Qt version:</b> %1" ).arg( QT_VERSION_STR ) );
    lAuthor->setText( tr( "<b>Author(s):</b> %1" ).arg( Alexandra::appAuthor ) );
    lLicense->setText( tr( "<b>License:</b> %1" ).arg( Alexandra::appLicense ) );
    lAddress->setText( tr( "<b>Address:</b> <a href=\"%1\">Github</a>" ).arg( Alexandra::appAddress ) );
}

void AboutWindow::AboutQt()
{
    QMessageBox::aboutQt( this->parentWidget() );
}

void AboutWindow::showEvent( QShowEvent* event )
{
    // random image from resources
    image->setPixmap( QPixmap( QString( ":/cats/%1" ).arg( rand() % 6 + 1 ) ) );
    event->accept();
}
