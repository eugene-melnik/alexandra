/*************************************************************************************************
 *                                                                                                *
 *  file: addfilmwindow.h                                                                         *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>                               *
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

#ifndef ADDFILMWINDOW_H
#define ADDFILMWINDOW_H


#include "ui_addfilmwindow.h"
#include "alexandrasettings.h"
#include "filmslist/filmitem.h"
#include "parsers/parsermanager.h"
#include "tools/debug.h"


#include <QDialog>


class AddFilmWindow : public QDialog, protected Ui::AddFilmWindow
{
    Q_OBJECT

    public:
        explicit AddFilmWindow( QWidget* parent = nullptr );
        virtual ~AddFilmWindow();

    signals:
        void Done( FilmItem* );

    protected:
        int viewsCount = 0;
        QString viewingDates = "";
        QString filmNewPosterName;
        AlexandraSettings* settings;

    protected slots:
        void OpenFilm();
        void OpenPosterFileClicked();
        void LoadInformation();

        void EditStarring() { ShowEditor( lStarringText, tr("Starring") ); }
        void EditDescription() { ShowEditor( lDescriptionText, tr("Description") ); }

        void ShowEditor( CutLabel* editDataIn, const QString& title );

        bool CanBeSaved();
        void OkClicked();
        void Save();

        void InformationLoaded( const FilmItem& film, const QString& posterFileName );
        void InformationLoadError( const QString& error );

    private:
        ParserManager* parser;
};


#endif // ADDFILMWINDOW_H

