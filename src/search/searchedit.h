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

#include <QAbstractItemModel>
#include <QAction>
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

        void SetModel( QAbstractItemModel* model );

    signals:
        void TextChanged( const QString& text, QList<int> selectedColumns );

    private slots:
        void SetupMenu();

        void ShowMenu();
        void CalculateOptions();
        void SelectAllOptions() { SetOptionsChecked( true ); }
        void UnselectAllOptions() { SetOptionsChecked( false ); }
        void SetOptionsChecked( bool b );

    private:
        QAction* actionSettings;
        QList<QAction*> actionsColumns;
        SearchEditMenu* menuSelectColumns;
        QList<int> selectedColumns;

        QAbstractItemModel* sourceModel;
};

class SearchEditMenu : public QMenu
{
    Q_OBJECT

    public:
        SearchEditMenu( QWidget* parent = nullptr ) : QMenu( parent ) {}

    protected:
          // Prevent a menu from closing when one of its actions was triggered
        void mouseReleaseEvent( QMouseEvent* event )
        {
            QAction* action = activeAction();

            if( action && action->isEnabled() )
            {
                action->setEnabled( false );
                QMenu::mouseReleaseEvent( event );
                action->setEnabled( true );
                action->trigger();
            }
            else
            {
                QMenu::mouseReleaseEvent( event );
            }
        }
};

#endif // SEARCHEDIT_H
