
#include "effects/effectdropshadow.h"
#include "filmsviewgriditem.h"

#include <QFileInfo>
#include <QPixmap>
#include <QString>
#include <QVBoxLayout>

FilmViewGridItem::FilmViewGridItem( const Film* film, AlexandraSettings* s, QWidget* parent )
    : QWidget( parent )
{
    titleText = film->GetTitle();

    // Film information
    if( s->GetGridViewShowTooltip() == true )
    {
        QString tooltip = film->GetTitle() + "<br/>";
        tooltip += tr( "<b>Original title:</b> %1" ).arg( film->GetOriginalTitle() ) + "<br/>";
        tooltip += tr( "<b>Genre:</b> %1" ).arg( film->GetGenre() ) + "<br/>";
        tooltip += tr( "<b>Year:</b> %1" ).arg( film->GetYearStr() ) + "<br/>";
        tooltip += tr( "<b>Country:</b> %1" ).arg( film->GetCountry() ) + "<br/>";
        tooltip += tr( "<b>Director:</b> %1" ).arg( film->GetDirector() ) + "<br/>";
        tooltip += tr( "<b>Producer:</b> %1" ).arg( film->GetProducer() ) + "<br/>";
        tooltip += tr( "<b>Starring:</b> %1" ).arg( film->GetStarring() ) + "<br/>";
        tooltip += tr( "<b>Rating:</b> %1" ).arg( film->GetRatingStr() ) + "<br/>";
        tooltip += tr( "<b>Description:</b> %1" ).arg( film->GetDescription() ) + "<br/>";
        tooltip += tr( "<b>Tags:</b> %1" ).arg( film->GetTags() ) + "<br/>";
        setToolTip( tooltip );
    }

    // Film poster
    poster = new QLabel( this );
    poster->setAlignment( Qt::AlignCenter );

    QString posterFilePath = s->GetPostersDirPath() + "/" + film->GetPosterName();

    if( !film->GetIsPosterExists() || !QFileInfo( posterFilePath ).exists() )
    {
        posterFilePath = ":/standart-poster";
    }

    int itemSize = s->GetGridViewItemSize();
    poster->setPixmap( QPixmap( posterFilePath ).scaledToHeight( itemSize, Qt::SmoothTransformation ) );
    poster->setGraphicsEffect( new EffectDropShadow( 0, 0, 10, poster ) );

    // Film title
    title = new QLabel( this );
    title->setAlignment( Qt::AlignCenter );
    title->setStyleSheet( QString( "font-size: %1px" ).arg( s->GetGridViewTextSize() ) );
    title->setWordWrap( true );

    int textLenght = s->GetGridViewTextLength();

    if( film->GetTitle().length() > textLenght )
    {
        title->setText( film->GetTitle().left( textLenght ) + "..." );
    }
    else
    {
        title->setText( film->GetTitle() );
    }

    // Main layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->insertWidget( 0, poster );
    layout->insertWidget( 1, title );
    setLayout( layout );
}

QString FilmViewGridItem::GetTitle() const
{
    return( titleText );
}
