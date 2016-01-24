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

        void setSelectionModel( QItemSelectionModel* selectionModel ) override;

        QRect visualRect( const QModelIndex& ) const override { return( rect() ); }
        void scrollTo( const QModelIndex&, ScrollHint ) override {}
        QModelIndex indexAt( const QPoint& ) const override { return( currentIndex() ); }

    protected:
        int horizontalOffset() const override { return( 0 ); }
        int verticalOffset() const override { return( 0 ); }

        bool isIndexHidden( const QModelIndex& ) const override { return( false ); }

        void setSelection( const QRect&, QItemSelectionModel::SelectionFlags ) override {}
        QRegion visualRegionForSelection( const QItemSelection& ) const override { return( rect() ); }

        QModelIndex	moveCursor( CursorAction, Qt::KeyboardModifiers ) override { return( currentIndex() ); }

    private slots:
        void ShowSelected( const QModelIndex& current, const QModelIndex& /* previous */ );

    private:
        QList< QPair<FilmsListModel::Columns,QLabel*> > textItems;
};

#endif // FILMINFOVIEW_H
