/*************************************************************************************************
 *                                                                                                *
 *  file: searchedit.h                                                                            *
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

#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QAction>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
#include <QMenu>
#include <QPoint>

class SearchEditMenu;

class SearchEdit : public QLineEdit
{
    Q_OBJECT

    public:
        explicit SearchEdit( QWidget* parent = nullptr );

        void LoadSettings();
        void SaveSettings() const;

        enum FilterBy {
            None         = 0,
            Title        = 0b0000000001,
            Tags         = 0b0000000010,
            Genre        = 0b0000000100,
            Starring     = 0b0000001000,
            Director     = 0b0000010000,
            Producer     = 0b0000100000,
            Screenwriter = 0b0001000000,
            Composer     = 0b0010000000,
            Country      = 0b0100000000,
            Description  = 0b1000000000,
            All = Title|Tags|Genre|Starring|Director|
                  Producer|Screenwriter|Composer|Country|Description
        };

    signals:
        void TextChanged( const QString& text, FilterBy fields );

    private slots:
        void SetupMenu();

        void ShowMenu();
        void CalculateOptions();
        void SelectAllOptions() { SetOptionsChecked( true ); }
        void UnselectAllOptions() { SetOptionsChecked( false ); }
        void SetOptionsChecked( bool b );

    private:
        typedef struct {
            FilterBy filter;
            QAction* action;
        } FilterAction;

        QList<FilterAction> filterActions;

        QAction* aOptions = nullptr;
        SearchEditMenu* mSelectFields = nullptr;
        FilterBy selectedFilters;
};

class SearchEditMenu : public QMenu
{
    Q_OBJECT

    public:
        SearchEditMenu( QWidget* parent = nullptr ) : QMenu( parent ) {}

    protected:
        // Prevent a menu from closing when
        // one of its actions is triggered
        void mouseReleaseEvent( QMouseEvent* event );
};

#endif // SEARCHEDIT_H
