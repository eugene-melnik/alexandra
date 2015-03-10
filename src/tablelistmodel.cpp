/*************************************************************************************************
 *                                                                                                *
 *  file: tablelistmodel.cpp                                                                      *
 *                                                                                                *
 *  Alexandra Video Library                                                                       *
 *  Copyright (C) 2015 Eugene Melnik <jeka7js@gmail.com>                                          *
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

#include "tablelistmodel.h"

TableListModel::TableListModel( QObject* parent ) : QStandardItemModel( parent )
{
    // Rows setup
    this->setRowCount( 0 );

    // Columns setup
    this->setColumnCount( colNames.size() );

    for( int i = 0; i < this->columnCount(); i++ ) {
        QStandardItem* item = new QStandardItem( colNames.at(i) );
        item->setToolTip( colTooltips.at(i) );
        this->setVerticalHeaderItem( i, item );
    }
}

/*
    twFilms->setContextMenuPolicy(Qt::CustomContextMenu);
    twFilms->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twFilms->setAlternatingRowColors(true);
    twFilms->setSelectionMode(QAbstractItemView::SingleSelection);
    twFilms->setSelectionBehavior(QAbstractItemView::SelectRows);
    twFilms->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    twFilms->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    twFilms->setSortingEnabled(true);
    twFilms->horizontalHeader()->setHighlightSections(false);
    twFilms->horizontalHeader()->setMinimumSectionSize(20);
    twFilms->horizontalHeader()->setStretchLastSection(true);
    twFilms->verticalHeader()->setVisible(false);
    twFilms->verticalHeader()->setDefaultSectionSize(20);
*/
