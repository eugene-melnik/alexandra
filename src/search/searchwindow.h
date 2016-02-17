/*************************************************************************************************
 *                                                                                                *
 *  file: searchwindow.h                                                                          *
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

#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H


#include "advancedsearchproxymodel.h"
#include "filmslist/filmitem.h"
#include "ui_searchwindow.h"


#include <list>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QDialog>
#include <QList>


class SearchWindow : public QDialog, protected Ui::SearchWindow
{
    Q_OBJECT

    public:
        explicit SearchWindow( QWidget* parent = nullptr );
        ~SearchWindow();

        void SetModel( QAbstractItemModel* model );

    signals:
        void ShowSelectedItem( QModelIndex index );

    private slots:
          // Simple search
        void ConfigureSimpleSearchTab();
        void SelectAll() { SetOptionsIsChecked( true ); }
        void UnselectAll() { SetOptionsIsChecked( false ); }
        void SetOptionsIsChecked( bool isChecked );
        void SimpleSearchStart();

          // Advanced search
        void ConfigureAdvancedSearchTab();
        void AdvancedSearchStart();
        void SetupAdvancedProxy( AdvancedSearchProxyModel* proxy );

          // Global
        void SetupModel( QAbstractProxyModel* proxy );
        void SetupResultHeader();
        void ShowInMainWindow();

    private:
        QAbstractItemModel* model = nullptr;
        QAbstractProxyModel* proxyModel = nullptr;
};


#endif // SEARCHWINDOW_H

