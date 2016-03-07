/*************************************************************************************************
 *                                                                                                *
 *  file: checkvieweddialog.h                                                                     *
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

#ifndef CHECKVIEWEDDIALOG_H
#define CHECKVIEWEDDIALOG_H


#include <QStringList>


#include "ui_checkvieweddialog.h"


class CheckViewedDialog : public QDialog, protected Ui::CheckViewedDialog
{
    Q_OBJECT

    public:
        CheckViewedDialog( const QStringList& list, QWidget* parent = nullptr ) : QDialog( parent )
        {
            setupUi(this);
            connect( bSelectAll, &QPushButton::clicked,       lwMain, &CheckedListWidget::SelectAll );
            connect( bUnselectAll, &QPushButton::clicked,     lwMain, &CheckedListWidget::UnselectAll );
            connect( bInvertSelection, &QPushButton::clicked, lwMain, &CheckedListWidget::InvertSelection );
            lwMain->AddItems( list );
        }

        QStringList Exec()
        {
            if( QDialog::exec() == QDialog::Accepted )
            {
                return( lwMain->GetSelectedItems() );
            }
            else
            {
                return( QStringList() );
            }
        }
};


#endif // CHECKVIEWEDDIALOG_H

