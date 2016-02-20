/*************************************************************************************************
 *                                                                                                *
 *  file: filmtechinfoview.h                                                                      *
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

#ifndef FILMTECHINFOVIEW_H
#define FILMTECHINFOVIEW_H


#include "abstractfilminfoview.h"
#include "alexandrasettings.h"


#include <QLabel>
#include <QMutex>


class FilmTechInfoView : public QLabel, public AbstractFilmInfoView
{
    Q_OBJECT

    public:
        explicit FilmTechInfoView( QWidget* parent = nullptr );
        ~FilmTechInfoView();

    public slots:
        void ShowInformation( const QModelIndex& index ) override;
        void Clear() override { clear(); repaint(); }

    signals:
        void ShortInfoLoaded( const QString& );

    protected slots:
        void resizeEvent( QResizeEvent* event );
        void showEvent( QShowEvent* event );

    private slots:
        void LoadTechnicalInfo( const QString& fileName );
        void ShowShortInfo( const QString& text ) { setText( text ); }

    private:
        AlexandraSettings* settings;
        QMutex mutexInfoLoad;
};


#endif // FILMTECHINFOVIEW_H

