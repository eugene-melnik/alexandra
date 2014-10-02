#include "aboutwindow.h"
#include "version.h"

AboutWindow::AboutWindow( QWidget* parent ) : QDialog( parent )
{
    setupUi( this );

    lAppName->setText( Alexandra::appNameGui );
    lAppVersion->setText( tr( "version %1" ).arg( Alexandra::appVersionFull ) );
    lAuthor->setText( tr( "<b>Author(s):</b> %1" ).arg( Alexandra::appAuthor ) );
    lLicense->setText( tr( "<b>License:</b> %1" ).arg( Alexandra::appLicense ) );
    lAddress->setText( tr( "<b>Address:</b> %1" ).arg( Alexandra::appAddress ) );

    // random image from resources
    image->setPixmap( QPixmap( QString( ":/cats/%1" ).arg( rand() % 6 + 1 ) ) );
}
