/*************************************************************************************************
 *                                                                                                *
 *  file: playlistwidget.h                                                                        *
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

#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H


#include <QList>
#include <QPair>
#include <QStringList>
#include <QWidget>


#include "ui_playlistwidget.h"


typedef QPair<QString,QString> PlaylistPair;
typedef QList<PlaylistPair> PlaylistPairList;


class PlaylistWidget : public QWidget, protected Ui::PlaylistWidget
{
    Q_OBJECT

    public:
        explicit PlaylistWidget( QWidget* parent = nullptr );

        void LoadSettings();
        void SaveSettings() const;

    public slots:
        void AddItem( const QString& title, const QString& filePath );
        void RemoveItem( const QString& title );
        void Clear() { lwMain->clear(); emit Cleared(); }

        QStringList GetTitles() const;
        QStringList GetPathes() const;
        bool IsEmpty() const { return( !lwMain->count() ); }

    signals:
        void Cleared();
        void ItemSelected( const QString& title );

    private slots:
        void ShowContextMenu( const QPoint& pos );
        void SelectClickedItem( const QModelIndex& index );
        void RemoveCurrentFromList();
};


#endif // PLAYLISTWIDGET_H

