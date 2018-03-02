/*************************************************************************************************
 *                                                                                                *
 *  file: statusbar.h                                                                             *
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

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QLabel>
#include <QStatusBar>

class StatusBar : public QStatusBar
{
    Q_OBJECT

    public:
        explicit StatusBar( QWidget* parent = nullptr );
        virtual ~StatusBar() = default;

        void setFont( const QFont& font );

    public slots:
        void ShowTotal( int total, int viewed, int favourite );
        void SetShown( int shown );

        void SetAdditionalText( const QString& text ) { additionalText->setText( text ); }

    private:
        QLabel* mainText;
        QLabel* separator;
        QLabel* shownText;
        QLabel* additionalText;

        int totalCount = 0;
};

#endif // STATUSBAR_H
