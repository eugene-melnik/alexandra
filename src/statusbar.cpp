/*************************************************************************************************
 *                                                                                                *
 *  file: statusbar.cpp                                                                           *
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

#include "statusbar.h"

StatusBar::StatusBar( QWidget* parent ) : QStatusBar( parent )
{
    setStyleSheet( "QStatusBar::item { border: 0px solid black };" );

    text = new QLabel( this );
    addWidget( text );

    progress = new QProgressBar( this );
    progress->setAlignment( Qt::AlignCenter );
    progress->setTextVisible( false );
    progress->hide();
    addPermanentWidget( progress );
}

StatusBar::~StatusBar()
{
    delete text;
    delete progress;
}

void StatusBar::ShowLoading()
{
    progress->setMaximumHeight( height() / 2 );
    progress->setMaximumWidth( 200 );
    progress->setValue( 0 );
    progress->show();

    text->setText( tr( "Loading..." ) );
    repaint();
}

void StatusBar::ShowFounded( int founded )
{
    progress->hide();
    text->setText( tr( "Founded: %1" ).arg( founded ) );
}

void StatusBar::ShowTotal( int total, int viewed, int favourite )
{
    progress->hide();
    text->setText( tr( "Total films: %1 (%2 viewed, %3 favourite)" )
                   .arg( total )
                   .arg( viewed )
                   .arg( favourite ) );
}
