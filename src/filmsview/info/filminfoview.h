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


#include <QWidget>


#include "abstractfilminfoview.h"
#include "filmslist/filmitem.h"
#include "ui_filminfoview.h"


class FilmInfoView : public QWidget, public AbstractFilmInfoView, protected Ui::FilmInfoView
{
    Q_OBJECT

    public:
        explicit FilmInfoView( QWidget* parent = nullptr );

    public slots:
        void ShowInformation( const QModelIndex& index ) override;
        void Clear() override;

        void ShowEmptyDatabaseMessage();
        void ShowMessage( const QString& title, const QString& message = QString() );

    private:
        QList< QPair<FilmItem::Column,QLabel*> > textItems;
};


#endif // FILMINFOVIEW_H

