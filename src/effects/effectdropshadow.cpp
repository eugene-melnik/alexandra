/*************************************************************************************************
 *                                                                                                *
 *  file: effectdropshadow.cpp                                                                    *
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

#include "effectdropshadow.h"


EffectDropShadow::EffectDropShadow( QObject* parent )
    : QGraphicsDropShadowEffect( parent )
{
    setColor( QColor( "black" ) );
    setBlurRadius( 5 );
    setOffset( 3, 3 );
}


EffectDropShadow::EffectDropShadow( qreal offsetX, qreal offsetY, const QColor& color, QObject* parent )
    : QGraphicsDropShadowEffect( parent )
{
    setOffset( offsetX, offsetY );
    setBlurRadius( 5 );
    setColor( QColor(color) );
}


EffectDropShadow::EffectDropShadow( qreal offsetX, qreal offsetY, qreal blurRadius, const QColor& color, QObject *parent )
    : QGraphicsDropShadowEffect( parent )
{
    setOffset( offsetX, offsetY );
    setBlurRadius( blurRadius );
    setColor( QColor(color) );
}

