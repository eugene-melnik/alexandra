/*************************************************************************************************
 *                                                                                                *
 *  file: filmsviewgridmodel.cpp                                                                  *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>                                     *
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

#include "filmsviewgridmodel.h"

#include <QFileInfo>

FilmViewGridModel::FilmViewGridModel(  AlexandraSettings* s, QObject* parent )
    : QAbstractListModel( parent ), settings( s )
{ }

void FilmViewGridModel::AppendItem( const Film& film, QColor background )
{
    itemTitle.append( QString() );
    itemImage.append( QPixmap() );
    itemToolTip.append( QString() );
    itemBackground.append( QColor() );

    SetItem( itemTitle.size() - 1, film, background );
}

void FilmViewGridModel::SetItem( int n, const Film& film, QColor background )
{
    // Film title
    itemTitle[ n ] = film.GetTitle();

    // Film poster
    QString posterFilePath = settings->GetPostersDirPath() + "/" + film.GetPosterName();

    if( !film.GetIsPosterExists() || !QFileInfo( posterFilePath ).exists() )
    {
        posterFilePath = ":/standart-poster";
    }

    itemImage[ n ] = QPixmap( posterFilePath ).scaledToHeight( settings->GetGridItemSize(),
                                                               Qt::SmoothTransformation );

    // Film information
    QString tooltip;

    if( settings->GetGridShowTooltip() == true )
    {
        tooltip = film.GetTitle() + "<br/>";
        tooltip += tr( "<b>Original title:</b> %1" ).arg( film.GetOriginalTitle() ) + "<br/>";
        tooltip += tr( "<b>Genre:</b> %1" ).arg( film.GetGenre() ) + "<br/>";
        tooltip += tr( "<b>Year:</b> %1" ).arg( film.GetYearStr() ) + "<br/>";
        tooltip += tr( "<b>Country:</b> %1" ).arg( film.GetCountry() ) + "<br/>";
        tooltip += tr( "<b>Director:</b> %1" ).arg( film.GetDirector() ) + "<br/>";
        tooltip += tr( "<b>Producer:</b> %1" ).arg( film.GetProducer() ) + "<br/>";
        tooltip += tr( "<b>Starring:</b> %1" ).arg( film.GetStarring() ) + "<br/>";
        tooltip += tr( "<b>Rating:</b> %1" ).arg( film.GetRatingStr() ) + "<br/>";
        tooltip += tr( "<b>Description:</b> %1" ).arg( film.GetDescription() ) + "<br/>";
        tooltip += tr( "<b>Tags:</b> %1" ).arg( film.GetTags() ) + "<br/>";
    }

    itemToolTip[ n ] = tooltip;

    // Background color
    itemBackground[ n ] = background;

    emit layoutChanged(); // update the layout
}

void FilmViewGridModel::Clear()
{
    itemImage.clear();
    itemTitle.clear();
    itemToolTip.clear();
    itemBackground.clear();
}

void FilmViewGridModel::RemoveRow( int row )
{
    QAbstractListModel::removeRow( row );
    itemImage.removeAt( row );
    itemTitle.removeAt( row );
    itemToolTip.removeAt( row );
    itemBackground.removeAt( row );

    emit layoutChanged(); // update the layout
}

int FilmViewGridModel::GetItemIndexByTitle( QString title )
{
    return( itemTitle.indexOf( title ) );
}

QString FilmViewGridModel::GetItemTitle( int n )
{
    return( itemTitle.at( n ) );
}

int FilmViewGridModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent )
    return( itemTitle.size() );
}

QVariant FilmViewGridModel::data( const QModelIndex& index, int role ) const
{
    int row = index.row();

    switch( role )
    {
        case Qt::DisplayRole :
        {
            return( itemTitle.at( row ) );
        }

        case Qt::DecorationRole :
        {
            return( itemImage.at( row ) );
        }

        case Qt::ToolTipRole :
        {
            return( itemToolTip.at( row ) );
        }

        case Qt::BackgroundColorRole :
        {
            if( itemBackground.at( row ).isValid() )
            {
                return( itemBackground.at( row ) );
            }
            else
            {
                return( QVariant() );
            }
        }

        case Qt::SizeHintRole :
        {
            int width = settings->GetGridItemSize() + settings->GetGridItemSize() * 0.1; // +10%
            int height = settings->GetGridItemSize() + settings->GetGridFontSize() * 2;  // +2* font size
            return( QSize( width, height ) );
        }

        default:
        {
            return( QVariant() );
        }
    }
}
