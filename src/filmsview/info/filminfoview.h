/*************************************************************************************************
 *                                                                                                *
 *  file: filminfoview.h                                                                        *
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

#ifndef FILMINFOVIEW_H
#define FILMINFOVIEW_H

#include <QAbstractItemView>

#include "filmslist/filmslistmodel.h"
#include "ui_filminfoview.h"

class FilmInfoView : public QAbstractItemView, protected Ui::FilmInfoView
{
    Q_OBJECT

    public:
        explicit FilmInfoView( QWidget* parent = nullptr );

        void Clear();
        void ShowEmptyDatabaseMessage() const;
        void ShowMessage( const QString& message ) { lFilmTitle->setText( message ); }

        void setFont( const QFont& font );

        void setSelectionModel( QItemSelectionModel* selectionModel );

        QRect visualRect( const QModelIndex& ) const { return( rect() ); }
        void scrollTo( const QModelIndex&, ScrollHint ) {}
        QModelIndex indexAt( const QPoint& ) const { return( currentIndex() ); }

    protected:
        int horizontalOffset() const { return( 0 ); }
        int verticalOffset() const { return( 0 ); }

        bool isIndexHidden( const QModelIndex& ) const { return( false ); }

        void setSelection( const QRect&, QItemSelectionModel::SelectionFlags ) {}
        QRegion visualRegionForSelection( const QItemSelection& ) const { return( rect() ); }

        QModelIndex	moveCursor( CursorAction, Qt::KeyboardModifiers ) { return( currentIndex() ); }

    private slots:
        void ShowSelected( const QModelIndex& current, const QModelIndex& /* previous */ );

    private:
        QList< QPair<FilmsListModel::Columns,QLabel*> > textItems;
};

#endif // FILMINFOVIEW_H
