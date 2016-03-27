/*************************************************************************************************
 *                                                                                                *
 *  file: filminfoview.cpp                                                                      *
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

#include "filminfoview.h"
#include "filmslist/filmslistproxymodel.h"
#include "tools/debug.h"

#include <QList>


FilmInfoView::FilmInfoView( QWidget* parent ) : QWidget( parent )
{
    setupUi( this );

    textItems =
    {
          // "first"                        // "second"
        { FilmItem::OriginalTitleColumn,  lOriginalTitle },
        { FilmItem::TaglineColumn,        lTagline },
        { FilmItem::GenreColumn,          lGenre },
        { FilmItem::YearColumn,           lYear },
        { FilmItem::BudgetColumn,         lBudget },
        { FilmItem::CountryColumn,        lCountry },
        { FilmItem::ScreenwriterColumn,   lScreenwriter },
        { FilmItem::DirectorColumn,       lDirector },
        { FilmItem::ProducerColumn,       lProducer },
        { FilmItem::ComposerColumn,       lComposer },
        { FilmItem::StarringColumn,       lStarring },
        { FilmItem::DescriptionColumn,    lDescription },
        { FilmItem::TagsColumn,           lTags }
    };

    Clear();
}


void FilmInfoView::ShowInformation( const QModelIndex& index )
{
    if( index.isValid() )
    {
        DebugPrintFunc( "FilmInfoView::ShowInformation", index.row() );
        const FilmsListProxyModel* model = static_cast<const FilmsListProxyModel*>( index.model() );
        QString pattern = "<b>%1:</b> %2";

        lFilmTitle->setText( model->index( index.row(), FilmItem::TitleColumn ).data().toString() );
        lFilmTitle->show();

        for( QPair<FilmItem::Column,QLabel*>& item : textItems )
        {
            QString title = model->headerData( item.first, Qt::Horizontal, Qt::DisplayRole ).toString();
            QString text = model->index( index.row(), item.first ).data().toString();
            item.second->setText( pattern.arg( title, text ) );
            item.second->setVisible( !text.isEmpty() );
        }

        QString title = model->headerData( FilmItem::RatingColumn, Qt::Horizontal, Qt::DisplayRole ).toString();
        QString text = model->index( index.row(), FilmItem::RatingColumn ).data( Qt::ToolTipRole ).toString();
        lRating->setText( pattern.arg( title, text ) );
        lRating->setVisible( !text.isEmpty() );

        QVariant pixmap = model->index( index.row(), FilmItem::RatingColumn ).data( Qt::DecorationRole );
        lRatingImg->setPixmap( pixmap.value<QPixmap>() );
        lRatingImg->setVisible( !text.isEmpty() );

        DebugPrintFuncDone( "FilmInfoView::ShowInformation" );
    }
}


void FilmInfoView::Clear()
{
    for( QLabel* label : this->findChildren<QLabel*>() )
    {
        label->clear();
        label->hide();
    }
}


void FilmInfoView::ShowEmptyDatabaseMessage()
{
    Clear();

    const QString title = tr( "Your database is empty" );
    lFilmTitle->setText( title );
    lFilmTitle->show();

    const QString line1 = tr( "Hi! At this point your database is empty. It's time to fill it! To do this, use the following tools:" );
    lOriginalTitle->setText( line1 + "<br/><br/>" );
    lOriginalTitle->show();

    const QString line2 = tr( "Adding movies one by one (menu \"Film\"→\"Add film\" or the \"Add\" button on the toolbar)." );
    lTagline->setText( "1. " + line2 );
    lTagline->show();

    const QString line3 = tr( "Scan the directories on your hard drive (menu \"Tools\"→\"Films scanner\" or button on the toolbar)." );
    lGenre->setText( "2. " + line3 );
    lGenre->show();

    const QString line4 = tr( "If you already have filled database, just select it in the settings (\"Application\" tab)." );
    lCountry->setText( "3. " + line4 );
    lCountry->show();

    const QString line5 = tr( "All of these tools have automatic information retrieval via the Internet as well as loading a graphic poster for the film. Enjoy!" );
    lScreenwriter->setText( line5 );
    lScreenwriter->show();
}


void FilmInfoView::ShowMessage( const QString& title, const QString& message )
{
    Clear();
    lFilmTitle->setText( title );
    lFilmTitle->show();
    lOriginalTitle->setText( message );
    lOriginalTitle->show();
}

