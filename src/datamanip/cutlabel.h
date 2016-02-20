/*************************************************************************************************
 *                                                                                                *
 *  file: cutlabel.h                                                                              *
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

#ifndef CUTLABEL_H
#define CUTLABEL_H


#include <QApplication>
#include <QFontMetrics>
#include <QLabel>


class CutLabel : public QLabel
{
    Q_OBJECT

    public:
        explicit CutLabel( QWidget* parent = nullptr ) : QLabel( parent ) {}

        QString text() const
        {
            if( fullString != EmptyString )
            {
                return( fullString );
            }
            else
            {
                return( QString() );
            }
        }

    public slots:
        void setText( const QString& text )
        {
            if( text.isEmpty() )
            {
                fullString = EmptyString;
            }
            else
            {
                fullString = text;
            }

            DisplayText();
        }

    protected:
        void resizeEvent( QResizeEvent* event ) override
        {
            QLabel::resizeEvent( event );
            DisplayText();
        }

        void DisplayText()
        {
            QFontMetrics fm( qApp->font() );
            QString displayString = fm.elidedText( fullString, Qt::ElideRight, this->width() );
            QLabel::setText( displayString );
        }

    private:
        QString fullString;

        const QString EmptyString = "-";
};


#endif // CUTLABEL_H

